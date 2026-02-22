// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

import SwiftUI

struct ContentView: View {
    @EnvironmentObject var connectionManager: ConnectionManager
    @State private var selectedTab = 0
    
    var body: some View {
        NavigationView {
            if connectionManager.isConnected {
                TabView(selection: $selectedTab) {
                    MediaControlView()
                        .tabItem {
                            Label("Media", systemImage: "music.note")
                        }
                        .tag(0)
                    
                    InputControlView()
                        .tabItem {
                            Label("Input", systemImage: "hand.point.up.left")
                        }
                        .tag(1)
                    
                    FileTransferView()
                        .tabItem {
                            Label("Files", systemImage: "doc")
                        }
                        .tag(2)
                    
                    SystemControlView()
                        .tabItem {
                            Label("System", systemImage: "power")
                        }
                        .tag(3)
                    
                    ScreenShareView()
                        .tabItem {
                            Label("Screen", systemImage: "display")
                        }
                        .tag(4)
                }
                .navigationTitle("PC Remote")
                .navigationBarTitleDisplayMode(.inline)
                .toolbar {
                    ToolbarItem(placement: .navigationBarTrailing) {
                        Button(action: {
                            connectionManager.disconnect()
                        }) {
                            Image(systemName: "xmark.circle.fill")
                                .foregroundColor(.red)
                        }
                    }
                }
            } else {
                ConnectView()
                    .navigationTitle("Connect to PC")
                    .navigationBarTitleDisplayMode(.inline)
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(ConnectionManager())
    }
}
