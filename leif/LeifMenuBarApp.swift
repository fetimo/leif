//
//  LeifMenuBarApp.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import SwiftUI
import AppKit
import Foundation

@main
struct LeifMenuBarApp: App {

    @NSApplicationDelegateAdaptor(AppDelegate.self) private var appDelegate

    var body: some Scene {
        WindowGroup {
            ContentView(vm: LeifViewModel()).frame(width: .zero)
        }
    }
}


class AppDelegate: NSObject, NSApplicationDelegate, ObservableObject {
    
    var statusItem: NSStatusItem!
    let unit = "gCO₂eq"
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {

        statusItem = NSStatusBar.system.statusItem(withLength: NSStatusItem.variableLength)
        if let button = statusItem.button {
//            button.title = "🍃"
            button.image = NSImage(systemSymbolName: "leaf.fill", accessibilityDescription: "leaf")
        }

        setupMenus()
    }

    func setupMenus() {
        // 1
        let menu = NSMenu()
        menu.autoenablesItems = false

        // 2
        let session = NSMenuItem(title: "Session: 0.0 \(unit)", action: #selector(noop), keyEquivalent: "")
        session.isEnabled = false
        menu.addItem(session)
        
        let total = NSMenuItem(title: "Lifetime: 0.0 \(unit)", action: #selector(noop) , keyEquivalent: "")
        total.isEnabled = false
        menu.addItem(total)
        
        let reset = NSMenuItem(title: "Reset stats", action: #selector(didTapReset) , keyEquivalent: "r")
        menu.addItem(reset)

        menu.addItem(NSMenuItem.separator())

        menu.addItem(NSMenuItem(title: "Quit", action: #selector(NSApplication.terminate(_:)), keyEquivalent: "q"))

        // 3
        statusItem.menu = menu
    }
    
    private func resetStats() {
        do {
            let path = try FileManager.default.url(
                for: .cachesDirectory,
                in: .userDomainMask,
                appropriateFor: nil,
                create: false
            )
            try FileManager.default.removeItem(atPath: path.path)
        } catch {
            print("ERROR DESCRIPTION: \(error)")
        }
    }
    
    func updateCurrentImpact(session: Float, overall: Float) {
        statusItem.menu?.items[0].title = "Session: \(String(format:"%.2f", session)) \(unit)"
        statusItem.menu?.items[1].title = "Lifetime: \(String(format:"%.2f", overall)) \(unit)"
    }

    @objc func noop() {}

    @objc func didTapReset() {
        resetStats()
    }
}
