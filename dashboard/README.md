# dashboard/

This is the **source copy** of the FlowSense-X dashboard.

You do **not** need to attach this file separately anywhere. A copy of this
exact HTML is already embedded inside the firmware itself, in
`src/web_dashboard.cpp` (inside the `DASHBOARD_HTML` raw string near the top
of the file). When you Build + Upload the project in PlatformIO, the
dashboard goes to the ESP32 automatically as part of the firmware — nothing
extra to upload, nothing extra to attach.

`flowsense-dashboard.html` lives here only so it's easy to open, preview in
a browser, and edit as plain HTML/CSS/JS without digging through C++ code.

## If you edit this file

The copy in `src/web_dashboard.cpp` will **not** update automatically —
paste the new HTML into the `DASHBOARD_HTML` raw string in that file
(between `R"FSXHTML(` and `)FSXHTML";`), then Build + Upload again.
