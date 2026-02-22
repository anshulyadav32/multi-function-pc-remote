// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

package com.pcremote.app.network

import com.google.gson.Gson
import okhttp3.*
import java.util.concurrent.TimeUnit

class RemoteConnection {
    private var webSocket: WebSocket? = null
    private val client = OkHttpClient.Builder()
        .connectTimeout(10, TimeUnit.SECONDS)
        .readTimeout(10, TimeUnit.SECONDS)
        .build()
    private val gson = Gson()
    
    fun connect(serverAddress: String) {
        val request = Request.Builder()
            .url("ws://$serverAddress")
            .build()
        
        webSocket = client.newWebSocket(request, object : WebSocketListener() {
            override fun onOpen(webSocket: WebSocket, response: Response) {
                println("Connected to server")
            }
            
            override fun onMessage(webSocket: WebSocket, text: String) {
                println("Received: $text")
            }
            
            override fun onFailure(webSocket: WebSocket, t: Throwable, response: Response?) {
                println("Connection failed: ${t.message}")
            }
            
            override fun onClosed(webSocket: WebSocket, code: Int, reason: String) {
                println("Connection closed")
            }
        })
    }
    
    fun disconnect() {
        webSocket?.close(1000, "User disconnected")
        webSocket = null
    }
    
    fun sendMediaCommand(action: String) {
        val command = mapOf(
            "type" to "media",
            "action" to action,
            "id" to System.currentTimeMillis()
        )
        sendCommand(command)
    }
    
    fun sendInputCommand(action: String, data: Map<String, Any> = emptyMap()) {
        val command = mutableMapOf(
            "type" to "input",
            "action" to action,
            "id" to System.currentTimeMillis()
        )
        command.putAll(data)
        sendCommand(command)
    }
    
    fun sendSystemCommand(action: String) {
        val command = mapOf(
            "type" to "system",
            "action" to action,
            "id" to System.currentTimeMillis()
        )
        sendCommand(command)
    }
    
    private fun sendCommand(command: Map<String, Any>) {
        val json = gson.toJson(command)
        webSocket?.send(json)
    }
}
