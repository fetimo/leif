//
//  ContentView.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import SwiftUI

struct ContentView: View {
    
    @EnvironmentObject var appDelegate: AppDelegate
    @StateObject private var vm: LeifViewModel
    
    init(vm: LeifViewModel) {
        self._vm = StateObject(wrappedValue: vm)
    }
    
    func updateWatts() async {
        var battery = Battery()
        battery.open()
        let isCharging = battery.isACPowered()

        await vm.populateForecast()
        await vm.populateIntensity()
    
        /// If charging we want to start or continue to collect watt usage.
        if isCharging {
            var payload = vm.createOrFetchPreviousWatts()
            let watts = battery.getPowerDrawInWatts()
            payload = vm.addNewMeasurement(payload: payload, watts: watts)
            try? vm.storage.save(payload, for: "watts")
            
            /// Update the menu bar
            appDelegate.updateCurrentImpact(
                session: payload.total_session,
                overall: payload.total_overall,
                intensity: vm.intensity.data.index,
                forecast: vm.generateForecast()
            )
        } else {
            let payload = vm.removePreviousWatts()
            appDelegate.updateCurrentImpact(
                session: payload.total_session,
                overall: payload.total_overall,
                intensity: vm.intensity.data.index,
                forecast: vm.generateForecast()
            )
        }
        battery.close()
    }
    
    var body: some View {
        ScrollView {}.onReceive(vm.timer) { time in
            Task {
                await updateWatts()
            }
        }.onAppear() {
            Task {
                await updateWatts()
            }
        }.frame(width: .zero)
    }
}
