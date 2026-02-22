<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
<!-- Copyright (C) 2026 Multi-Function PC Remote Contributors -->

<template>
  <div class="remote-container">
    <div class="card" v-if="!connected">
      <h2>Connect to PC</h2>
      <div class="connect-form">
        <input 
          v-model="serverAddress" 
          type="text" 
          placeholder="192.168.1.100:8765"
          @keyup.enter="connect"
        />
        <button @click="connect" class="btn-primary">Connect</button>
      </div>
    </div>

    <div class="card control-panel" v-else>
      <div class="tabs">
        <button 
          v-for="tab in tabs" 
          :key="tab.id"
          :class="{ active: currentTab === tab.id }"
          @click="currentTab = tab.id"
        >
          {{ tab.name }}
        </button>
      </div>

      <div class="tab-content">
        <!-- Media Control -->
        <div v-if="currentTab === 'media'" class="media-controls">
          <h3>Media Control</h3>
          <div class="media-buttons">
            <button @click="sendMediaCommand('previous')" class="control-btn">⏮️ Previous</button>
            <button @click="sendMediaCommand('play_pause')" class="control-btn large">▶️ Play/Pause</button>
            <button @click="sendMediaCommand('next')" class="control-btn">⏭️ Next</button>
          </div>
          <div class="volume-controls">
            <button @click="sendMediaCommand('mute')" class="control-btn">🔇 Mute</button>
          </div>
        </div>

        <!-- Input Control -->
        <div v-if="currentTab === 'input'" class="input-controls">
          <h3>Mouse & Keyboard</h3>
          <div class="touchpad"
               @mousemove="handleMouseMove"
               @mousedown="handleMouseDown"
               @mouseup="handleMouseUp">
            <p>Touch here to move mouse</p>
          </div>
          <div class="mouse-buttons">
            <button @click="sendMouseClick('left')" class="control-btn">Left Click</button>
            <button @click="sendMouseClick('right')" class="control-btn">Right Click</button>
          </div>
        </div>

        <!-- File Transfer -->
        <div v-if="currentTab === 'files'" class="file-controls">
          <h3>File Transfer</h3>
          <input type="file" @change="handleFileSelect" ref="fileInput" style="display:none" />
          <button @click="$refs.fileInput.click()" class="control-btn large">
            📁 Select File to Send
          </button>
          <div v-if="selectedFile" class="file-info">
            <p>Selected: {{ selectedFile.name }}</p>
            <button @click="uploadFile" class="btn-primary">Upload</button>
          </div>
        </div>

        <!-- System Control -->
        <div v-if="currentTab === 'system'" class="system-controls">
          <h3>System Commands</h3>
          <div class="system-buttons">
            <button @click="sendSystemCommand('lock')" class="control-btn">🔒 Lock</button>
            <button @click="sendSystemCommand('sleep')" class="control-btn">💤 Sleep</button>
            <button @click="sendSystemCommand('restart')" class="control-btn warning">🔄 Restart</button>
            <button @click="sendSystemCommand('shutdown')" class="control-btn danger">⚡ Shutdown</button>
          </div>
        </div>

        <!-- Screen Share -->
        <div v-if="currentTab === 'screen'" class="screen-controls">
          <h3>Screen Sharing</h3>
          <button @click="toggleScreenShare" class="control-btn large">
            {{ screenSharing ? '⏹️ Stop Sharing' : '▶️ Start Sharing' }}
          </button>
          <div v-if="screenSharing" class="screen-preview">
            <img :src="screenImage" alt="Screen preview" />
          </div>
        </div>
      </div>

      <button @click="disconnect" class="btn-disconnect">Disconnect</button>
    </div>
  </div>
</template>

<script>
export default {
  name: 'RemoteControl',
  data() {
    return {
      serverAddress: 'localhost:8765',
      connected: false,
      ws: null,
      currentTab: 'media',
      tabs: [
        { id: 'media', name: 'Media' },
        { id: 'input', name: 'Input' },
        { id: 'files', name: 'Files' },
        { id: 'system', name: 'System' },
        { id: 'screen', name: 'Screen' }
      ],
      selectedFile: null,
      screenSharing: false,
      screenImage: '',
      lastMousePos: { x: 0, y: 0 }
    }
  },
  methods: {
    connect() {
      this.ws = new WebSocket(`ws://${this.serverAddress}`)
      
      this.ws.onopen = () => {
        this.connected = true
        console.log('Connected to server')
      }
      
      this.ws.onmessage = (event) => {
        const data = JSON.parse(event.data)
        console.log('Received:', data)
        
        if (data.type === 'screen' && data.action === 'frame') {
          this.screenImage = `data:image/jpeg;base64,${data.data}`
        }
      }
      
      this.ws.onclose = () => {
        this.connected = false
        console.log('Disconnected from server')
      }
      
      this.ws.onerror = (error) => {
        console.error('WebSocket error:', error)
        alert('Failed to connect to server')
      }
    },
    
    disconnect() {
      if (this.ws) {
        this.ws.close()
        this.ws = null
      }
      this.connected = false
      this.screenSharing = false
    },
    
    sendCommand(command) {
      if (this.ws && this.ws.readyState === WebSocket.OPEN) {
        this.ws.send(JSON.stringify({ ...command, id: Date.now() }))
      }
    },
    
    sendMediaCommand(action) {
      this.sendCommand({ type: 'media', action })
    },
    
    sendMouseClick(button) {
      this.sendCommand({ type: 'input', action: 'mouse_click', button })
    },
    
    sendSystemCommand(action) {
      if (action === 'shutdown' || action === 'restart') {
        if (!confirm(`Are you sure you want to ${action} the PC?`)) {
          return
        }
      }
      this.sendCommand({ type: 'system', action })
    },
    
    handleMouseMove(event) {
      if (event.buttons === 1) {
        const deltaX = event.clientX - this.lastMousePos.x
        const deltaY = event.clientY - this.lastMousePos.y
        this.sendCommand({ type: 'input', action: 'mouse_move', deltaX, deltaY })
      }
      this.lastMousePos = { x: event.clientX, y: event.clientY }
    },
    
    handleMouseDown(event) {
      this.lastMousePos = { x: event.clientX, y: event.clientY }
    },
    
    handleMouseUp() {
      // Mouse up
    },
    
    handleFileSelect(event) {
      this.selectedFile = event.target.files[0]
    },
    
    uploadFile() {
      if (!this.selectedFile) return
      
      const reader = new FileReader()
      reader.onload = (e) => {
        const base64Data = btoa(e.target.result)
        this.sendCommand({
          type: 'file',
          action: 'receive',
          filename: this.selectedFile.name,
          data: base64Data
        })
        this.selectedFile = null
      }
      reader.readAsBinaryString(this.selectedFile)
    },
    
    toggleScreenShare() {
      this.screenSharing = !this.screenSharing
      this.sendCommand({
        type: 'screen',
        action: this.screenSharing ? 'start' : 'stop'
      })
    }
  }
}
</script>

<style scoped>
.remote-container {
  width: 100%;
  max-width: 600px;
}

.card {
  background: white;
  border-radius: 20px;
  padding: 2rem;
  box-shadow: 0 10px 40px rgba(0, 0, 0, 0.2);
}

.card h2 {
  margin-bottom: 1.5rem;
  color: #333;
  text-align: center;
}

.connect-form {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.connect-form input {
  padding: 1rem;
  border: 2px solid #e0e0e0;
  border-radius: 10px;
  font-size: 1rem;
}

.btn-primary {
  padding: 1rem;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  border: none;
  border-radius: 10px;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
  transition: transform 0.2s;
}

.btn-primary:hover {
  transform: translateY(-2px);
}

.tabs {
  display: flex;
  gap: 0.5rem;
  margin-bottom: 1.5rem;
  overflow-x: auto;
}

.tabs button {
  padding: 0.75rem 1.5rem;
  background: #f0f0f0;
  border: none;
  border-radius: 10px;
  cursor: pointer;
  transition: all 0.3s;
  white-space: nowrap;
}

.tabs button.active {
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
}

.tab-content {
  min-height: 300px;
}

.control-btn {
  padding: 1rem 1.5rem;
  background: #f5f5f5;
  border: 2px solid #e0e0e0;
  border-radius: 10px;
  cursor: pointer;
  transition: all 0.2s;
  font-size: 1rem;
}

.control-btn:hover {
  background: #e0e0e0;
  transform: translateY(-2px);
}

.control-btn.large {
  padding: 1.5rem 2rem;
  font-size: 1.2rem;
  font-weight: 600;
}

.control-btn.warning {
  background: #fff3cd;
  border-color: #ffc107;
}

.control-btn.danger {
  background: #f8d7da;
  border-color: #dc3545;
}

.media-controls {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.media-buttons {
  display: flex;
  gap: 1rem;
  justify-content: center;
  flex-wrap: wrap;
}

.volume-controls {
  display: flex;
  justify-content: center;
}

.input-controls {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.touchpad {
  width: 100%;
  height: 300px;
  background: #f5f5f5;
  border: 2px solid #e0e0e0;
  border-radius: 10px;
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: move;
  user-select: none;
}

.mouse-buttons {
  display: flex;
  gap: 1rem;
}

.file-controls {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.file-info {
  padding: 1rem;
  background: #f5f5f5;
  border-radius: 10px;
}

.system-controls {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.system-buttons {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 1rem;
}

.screen-controls {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.screen-preview {
  width: 100%;
  max-height: 400px;
  overflow: hidden;
  border-radius: 10px;
}

.screen-preview img {
  width: 100%;
  height: auto;
}

.btn-disconnect {
  margin-top: 2rem;
  width: 100%;
  padding: 1rem;
  background: #dc3545;
  color: white;
  border: none;
  border-radius: 10px;
  font-size: 1rem;
  font-weight: 600;
  cursor: pointer;
}

h3 {
  margin-bottom: 1rem;
  color: #333;
}
</style>
