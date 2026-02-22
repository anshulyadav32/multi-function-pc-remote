// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

package com.pcremote.app

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import com.pcremote.app.network.RemoteConnection
import com.pcremote.app.ui.theme.PCRemoteTheme

class MainActivity : ComponentActivity() {
    private lateinit var connection: RemoteConnection
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        
        connection = RemoteConnection()
        
        setContent {
            PCRemoteTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    RemoteControlScreen(connection)
                }
            }
        }
    }
    
    override fun onDestroy() {
        super.onDestroy()
        connection.disconnect()
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun RemoteControlScreen(connection: RemoteConnection) {
    var serverAddress by remember { mutableStateOf("192.168.1.100:8765") }
    var isConnected by remember { mutableStateOf(false) }
    var selectedTab by remember { mutableStateOf(0) }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("PC Remote") },
                colors = TopAppBarDefaults.topAppBarColors(
                    containerColor = MaterialTheme.colorScheme.primaryContainer
                )
            )
        }
    ) { padding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(padding)
                .padding(16.dp)
        ) {
            // Connection panel
            if (!isConnected) {
                OutlinedTextField(
                    value = serverAddress,
                    onValueChange = { serverAddress = it },
                    label = { Text("Server Address") },
                    modifier = Modifier.fillMaxWidth()
                )
                Spacer(modifier = Modifier.height(8.dp))
                Button(
                    onClick = {
                        connection.connect(serverAddress)
                        isConnected = true
                    },
                    modifier = Modifier.fillMaxWidth()
                ) {
                    Text("Connect")
                }
            } else {
                // Tabs
                TabRow(selectedTabIndex = selectedTab) {
                    Tab(
                        selected = selectedTab == 0,
                        onClick = { selectedTab = 0 },
                        text = { Text("Media") }
                    )
                    Tab(
                        selected = selectedTab == 1,
                        onClick = { selectedTab = 1 },
                        text = { Text("Input") }
                    )
                    Tab(
                        selected = selectedTab == 2,
                        onClick = { selectedTab = 2 },
                        text = { Text("System") }
                    )
                }
                
                Spacer(modifier = Modifier.height(16.dp))
                
                when (selectedTab) {
                    0 -> MediaControlPanel(connection)
                    1 -> InputControlPanel(connection)
                    2 -> SystemControlPanel(connection)
                }
                
                Spacer(modifier = Modifier.weight(1f))
                
                Button(
                    onClick = {
                        connection.disconnect()
                        isConnected = false
                    },
                    modifier = Modifier.fillMaxWidth(),
                    colors = ButtonDefaults.buttonColors(
                        containerColor = MaterialTheme.colorScheme.error
                    )
                ) {
                    Text("Disconnect")
                }
            }
        }
    }
}

@Composable
fun MediaControlPanel(connection: RemoteConnection) {
    Column(
        modifier = Modifier.fillMaxWidth(),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Row(
            horizontalArrangement = Arrangement.SpaceEvenly,
            modifier = Modifier.fillMaxWidth()
        ) {
            IconButton(onClick = { connection.sendMediaCommand("previous") }) {
                Icon(Icons.Default.SkipPrevious, "Previous", modifier = Modifier.size(48.dp))
            }
            IconButton(onClick = { connection.sendMediaCommand("play_pause") }) {
                Icon(Icons.Default.PlayArrow, "Play/Pause", modifier = Modifier.size(48.dp))
            }
            IconButton(onClick = { connection.sendMediaCommand("next") }) {
                Icon(Icons.Default.SkipNext, "Next", modifier = Modifier.size(48.dp))
            }
        }
        Spacer(modifier = Modifier.height(24.dp))
        Row(
            horizontalArrangement = Arrangement.SpaceEvenly,
            modifier = Modifier.fillMaxWidth()
        ) {
            IconButton(onClick = { connection.sendMediaCommand("mute") }) {
                Icon(Icons.Default.VolumeOff, "Mute")
            }
        }
    }
}

@Composable
fun InputControlPanel(connection: RemoteConnection) {
    Column(
        modifier = Modifier.fillMaxWidth(),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text("Touchpad", style = MaterialTheme.typography.titleMedium)
        Spacer(modifier = Modifier.height(8.dp))
        Surface(
            modifier = Modifier
                .fillMaxWidth()
                .height(300.dp),
            tonalElevation = 4.dp,
            shape = MaterialTheme.shapes.medium
        ) {
            // Touchpad area - would implement gesture detection
            Box(modifier = Modifier.fillMaxSize()) {
                Text(
                    "Touch here to move mouse",
                    modifier = Modifier.align(Alignment.Center)
                )
            }
        }
    }
}

@Composable
fun SystemControlPanel(connection: RemoteConnection) {
    Column(
        modifier = Modifier.fillMaxWidth(),
        verticalArrangement = Arrangement.spacedBy(12.dp)
    ) {
        Button(
            onClick = { connection.sendSystemCommand("lock") },
            modifier = Modifier.fillMaxWidth()
        ) {
            Icon(Icons.Default.Lock, "Lock")
            Spacer(modifier = Modifier.width(8.dp))
            Text("Lock PC")
        }
        Button(
            onClick = { connection.sendSystemCommand("sleep") },
            modifier = Modifier.fillMaxWidth()
        ) {
            Text("Sleep")
        }
        Button(
            onClick = { connection.sendSystemCommand("restart") },
            modifier = Modifier.fillMaxWidth(),
            colors = ButtonDefaults.buttonColors(
                containerColor = MaterialTheme.colorScheme.secondary
            )
        ) {
            Text("Restart")
        }
        Button(
            onClick = { connection.sendSystemCommand("shutdown") },
            modifier = Modifier.fillMaxWidth(),
            colors = ButtonDefaults.buttonColors(
                containerColor = MaterialTheme.colorScheme.error
            )
        ) {
            Icon(Icons.Default.PowerSettingsNew, "Shutdown")
            Spacer(modifier = Modifier.width(8.dp))
            Text("Shutdown")
        }
    }
}
