//
//  IntensityViewModel.swift
//  leif
//
//  Created by Tim Stone on 09/07/2022.
//

import Foundation

struct IntensityData {
    
    private var intensity: CO2Data
    
    init(intensity: CO2Data) {
        self.intensity = intensity
    }
    
    var data: Intensity {
        intensity.intensity
    }
    
    var id: Int {
        Int.random(in: 1..<5)
    }
}
