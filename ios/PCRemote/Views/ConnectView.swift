// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

import SwiftUI

struct ConnectView: View {
    @EnvironmentObject var connectionManager: ConnectionManager
    @State private var serverAddress = "192.168.1.100:8765"
    @State private var showError = false
    @State private var errorMessage = ""
    
    var body: some View {
        VStack(spacing: 24) {
            Image(systemName: "desktopcomputer")
                .font(.system(size: 80))
                .foregroundColor(.blue)
                .padding(.top, 40)
            
            Text("Connect to Your PC")
                .font(.title)
                .fontWeight(.bold)
            
            Text("Enter your PC's IP address and port")
                .font(.subheadline)
                .foregroundColor(.secondary)
            
            VStack(alignment: .leading, spacing: 8) {
                Text("Server Address")
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                TextField("192.168.1.100:8765", text: $serverAddress)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                    .autocapitalization(.none)
                    .disableAutocorrection(true)
                    .keyboardType(.URL)
            }
            .padding(.horizontal)
            
            Button(action: connect) {
                HStack {
                    Image(systemName: "network")
                    Text("Connect")
                        .fontWeight(.semibold)
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color.blue)
                .foregroundColor(.white)
                .cornerRadius(12)
            }
            .padding(.horizontal)
            .disabled(serverAddress.isEmpty)
            
            Spacer()
        }
        .alert("Connection Error", isPresented: $showError) {
            Button("OK", role: .cancel) { }
        } message: {
            Text(errorMessage)
        }
    }
    
    private func connect() {
        connectionManager.connect(to: serverAddress) { success, error in
            if !success {
                errorMessage = error ?? "Failed to connect"
                showError = true
            }
        }
    }
}

struct ConnectView_Previews: PreviewProvider {
    static var previews: some View {
        ConnectView()
            .environmentObject(ConnectionManager())
    }
}
