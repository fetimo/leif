#  leif

leif is an open-source macOS menu bar app to track your computer's CO2 equivalent emissions over time.

It does this by counting the average power consumption while your MacBook is charging. Data is saved locally and can be reset at any time.

CO2 data is provided by the National Grid via https://carbonintensity.org.uk.

I tried to use permacomputing principles while developing this by:

* going native, this [started as a Ruby script](https://gist.github.com/fetimo/bfddb1d35524106357e922cd64fcaa2a) but in a bid to minimise CPU and memory usage while running it I opted to use Swift instead
* making it clear when your computer is using energy contributing to climate change
* minimised dependencies, no libraries other than those provided by macOS

## Requirements and limitations

* UK only
* macOS 12+
* only tested on a MacBook Pro
* currently no binary exists, you'll have to build from source

## Work remaining

I need to refactor the code to work with older macOS versions (and see how feasible it is).
I want to add a forecasting feature or warning system if you're charging at a high impact time
