#!/bin/bash
cmake ..
make -j8
./doseAndActivity photogun.mac

python processing/doseRatePlot.py
python processing/activityPlot.py

python processing/doseRateTable.py
python processing/activityTable.py
