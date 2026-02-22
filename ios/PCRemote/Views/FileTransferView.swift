// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

import SwiftUI
import UniformTypeIdentifiers

struct FileTransferView: View {
    @EnvironmentObject var connectionManager: ConnectionManager
    @State private var showFilePicker = false
    @State private var selectedFileName: String?
    @State private var selectedFileData: Data?
    
    var body: some View {
        VStack(spacing: 24) {
            Spacer()
            
            Image(systemName: "doc.badge.arrow.up")
                .font(.system(size: 80))
                .foregroundColor(.blue)
            
            Text("File Transfer")
                .font(.title2)
                .fontWeight(.bold)
            
            Text("Send files to your PC")
                .font(.subheadline)
                .foregroundColor(.secondary)
            
            if let fileName = selectedFileName {
                VStack(spacing: 12) {
                    HStack {
                        Image(systemName: "doc.fill")
                            .foregroundColor(.blue)
                        Text(fileName)
                            .font(.subheadline)
                        Spacer()
                    }
                    .padding()
                    .background(Color.gray.opacity(0.1))
                    .cornerRadius(10)
                    
                    Button(action: uploadFile) {
                        HStack {
                            Image(systemName: "arrow.up.circle.fill")
                            Text("Upload File")
                                .fontWeight(.semibold)
                        }
                        .frame(maxWidth: .infinity)
                        .padding()
                        .background(Color.green)
                        .foregroundColor(.white)
                        .cornerRadius(12)
                    }
                }
                .padding()
            } else {
                Button(action: { showFilePicker = true }) {
                    VStack(spacing: 12) {
                        Image(systemName: "folder.badge.plus")
                            .font(.system(size: 40))
                        Text("Select File")
                            .fontWeight(.semibold)
                    }
                    .frame(maxWidth: .infinity)
                    .padding(40)
                    .background(Color.blue.opacity(0.1))
                    .foregroundColor(.blue)
                    .cornerRadius(20)
                    .overlay(
                        RoundedRectangle(cornerRadius: 20)
                            .stroke(Color.blue, style: StrokeStyle(lineWidth: 2, dash: [10]))
                    )
                }
                .padding()
            }
            
            Spacer()
        }
        .sheet(isPresented: $showFilePicker) {
            DocumentPicker { url in
                handleFileSelection(url)
            }
        }
    }
    
    private func handleFileSelection(_ url: URL) {
        selectedFileName = url.lastPathComponent
        
        if let data = try? Data(contentsOf: url) {
            selectedFileData = data
        }
    }
    
    private func uploadFile() {
        guard let fileName = selectedFileName,
              let fileData = selectedFileData else { return }
        
        let base64Data = fileData.base64EncodedString()
        
        connectionManager.sendCommand(
            type: "file",
            action: "receive",
            data: [
                "filename": fileName,
                "data": base64Data
            ]
        )
        
        // Reset selection
        selectedFileName = nil
        selectedFileData = nil
    }
}

struct DocumentPicker: UIViewControllerRepresentable {
    let onPick: (URL) -> Void
    
    func makeUIViewController(context: Context) -> UIDocumentPickerViewController {
        let picker = UIDocumentPickerViewController(forOpeningContentTypes: [.data, .image, .movie, .audio, .pdf])
        picker.delegate = context.coordinator
        return picker
    }
    
    func updateUIViewController(_ uiViewController: UIDocumentPickerViewController, context: Context) {}
    
    func makeCoordinator() -> Coordinator {
        Coordinator(onPick: onPick)
    }
    
    class Coordinator: NSObject, UIDocumentPickerDelegate {
        let onPick: (URL) -> Void
        
        init(onPick: @escaping (URL) -> Void) {
            self.onPick = onPick
        }
        
        func documentPicker(_ controller: UIDocumentPickerViewController, didPickDocumentsAt urls: [URL]) {
            guard let url = urls.first else { return }
            onPick(url)
        }
    }
}

struct FileTransferView_Previews: PreviewProvider {
    static var previews: some View {
        FileTransferView()
            .environmentObject(ConnectionManager())
    }
}
