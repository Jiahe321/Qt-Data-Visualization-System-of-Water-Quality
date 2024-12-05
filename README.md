# Lin, updated at 2024/11/29

## Notifications with some error
There may be some capitalization issues. 
I errorly write the `window.hpp` included in `main.cpp` and `window.cpp` as `Window.hpp`.
My compiler did not report any errors. If your compiler reports an error, just modify the header file in those two cpp file.
- I find this issue while using the Feng Linux machine. When I change this headers it can run successfully.

## To Write Your Piece of Code
- Modify the content in your `cpp` and `hpp`. Build a layout without slots first.
- If you want more challenge and complete your page, add more methods to `database.hpp` and `database.cpp`, and connect your charts to those methods (slots) to show the data.
- You can also change other code in `database.cpp` and `sample.hpp` if you think some values' data type is hard to use. (For example, I store time in db as string but maybe a time stamp is better)

## Other Notification
- Write a log of the thing you have done in this document.

# Task to do
## This and next week:
- Finish the layout of every page
 Successfully provide some complete charts on some pages
 Make One or two vedios for iterations.
  
## Remaining time:
- The extra page linked to google map API.
- Internationalisation
- Remaining vedio(s)
- Write the report

# Lin, updated at 2024/12/2
## Some changes
- `Data View` page can now search for certain determinand.
- (removed, all changes put into `database.cpp`)Add `methods.hpp` and `methods.cpp`. Now you can add query methods to those files instead of changing `database.hpp` or `cpp`.
- Add a method `createPollutantTrendChart` into `Methods` class. Only show data in chart, need more change to meet the coursework requirement.
- Try to apply this method in `pollutant overview` pages. Problem: chart cannot update after loading file.
- Try to emit a signal `dbUpdated` from `window.cpp` after database updated. This signal is not used yet.


# Lin, updated at 2024/12/5
## Some changes
- File structure changed. More clear to access to different code.
- Internationalization half down.
- Try to implement qml map for the Geographical Hotspots Page.
-- Write your log here --
