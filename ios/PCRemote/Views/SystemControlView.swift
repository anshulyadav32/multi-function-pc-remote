// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

import SwiftUI

struct SystemControlView: View {
    @EnvironmentObject var connectionManager: ConnectionManager
    @State private var showShutdownConfirm = false
    @State private var showRestartConfirm = false
    @State private var pendingAction: String?
    
    var body: some View {
        VStack(spacing: 20) {
            Text("System Commands")
                .font(.title2)
                .fontWeight(.bold)
                .padding(.top)
            
            Text("Control your PC's power state")
                .font(.subheadline)
                .foregroundColor(.secondary)
            
            VStack(spacing: 16) {
                SystemButton(
                    icon: "lock.fill",
                    title: "Lock PC",
                    color: .blue
                ) {
                    sendSystemCommand("lock")
                }
                
                SystemButton(
                    icon: "moon.fill",
                    title: "Sleep",
                    color: .indigo
                ) {
                    sendSystemCommand("sleep")
                }
                
                SystemButton(
                    icon: "arrow.clockwise",
                    title: "Restart",
                    color: .orange
                ) {
                    pendingAction = "restart"
                    showRestartConfirm = true
                }
                
                SystemButton(
                    icon: "power",
                    title: "Shutdown",
                    color: .red
                ) {
                    pendingAction = "shutdown"
                    showShutdownConfirm = true
                }
            }
            .padding()
            
            Spacer()
        }
        .alert("Confirm Shutdown", isPresented: $showShutdownConfirm) {
            Button("Cancel", role: .cancel) { }
            Button("Shutdown", role: .destructive) {
                sendSystemCommand("shutdown")
            }
        } message: {
            Text("Are you sure you want to shutdown your PC?")
        }
        .alert("Confirm Restart", isPresented: $showRestartConfirm) {
            Button("Cancel", role: .cancel) { }
            Button("Restart", role: .destructive) {
                sendSystemCommand("restart")
            }
        } message: {
            Text("Are you sure you want to restart your PC?")
        }
    }
    
    private func sendSystemCommand(_ action: String) {
        connectionManager.sendCommand(type: "system", action: action)
    }
}

struct SystemButton: View {
    let icon: String
    let title: String
    let color: Color
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            HStack {
                Image(systemName: icon)
                    .font(.title2)
                    .frame(width: 40)
                Text(title)
                    .font(.headline)
                Spacer()
                Image(systemName: "chevron.right")
                    .font(.caption)
                    .foregroundColor(.gray)
            }
            .padding()
            .frame(maxWidth: .infinity)
            .background(color.opacity(0.1))
            .foregroundColor(color)
            .cornerRadius(12)
        }
    }
}

struct SystemControlView_Previews: PreviewProvider {
    static var previews: some View {
        SystemControlView()
            .environmentObject(ConnectionManager())
    }
}
