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
        
        let intensity = NSMenuItem(title: "Current intensity: unknown", action: #selector(noop) , keyEquivalent: "")
        intensity.isEnabled = false
        menu.addItem(intensity)
        
        let secondMenuItem = NSMenuItem()
        secondMenuItem.title = "Region"
        
        let secondSubMenuItem = NSMenuItem()
        let secondSubMenu = NSMenu()
        secondSubMenu.autoenablesItems = true
        regions.forEach { (key: Int, value: String) in
            let subMenuItem = NSMenuItem()
            subMenuItem.title = value
            subMenuItem.isEnabled = true
            secondSubMenuItem.isEnabled = true
//            secondSubMenuItem.target = self
            secondSubMenuItem.action = #selector(didTapRegion)
            secondSubMenu.addItem(subMenuItem)
        }
        
        menu.addItem(secondMenuItem)
        menu.setSubmenu(secondSubMenu, for: secondMenuItem)
        
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
    
    func updateCurrentImpact(session: Float, overall: Float, intensity: String) {
        statusItem.menu?.items[0].title = "Session: \(String(format:"%.2f", session)) \(unit)"
        statusItem.menu?.items[1].title = "Lifetime: \(String(format:"%.2f", overall)) \(unit)"
        statusItem.menu?.items[2].title = "Current intensity: \(intensity)"
        
        if intensity == "high" || intensity == "very high", let button = statusItem.button {
            button.image = NSImage(systemSymbolName: "leaf", accessibilityDescription: "do not charge")
        } else if let button = statusItem.button {
            button.image = NSImage(systemSymbolName: "leaf.fill", accessibilityDescription: "charge")
        }
    }

    @objc func noop() {}

    @objc func didTapRegion(sender: Any) {
        print(sender)
    }
    
    @objc func didTapReset() {
        resetStats()
    }
}
