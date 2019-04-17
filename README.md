# Trello2Monthly
<h4 align="center">A command line program to build Monthly Status Report.</h4>

Trello2Monthly is a command line program to generate PDF and Word files from Trello. Trello2Monthly uses <a href="https://github.com/Microsoft/cpprestsdk">cpprestsdk</a> and <a href="https://github.com/gabime/spdlog">spdlog</a>, <a href="https://github.com/skystrife/cpptoml">cpptoml</a> and <a href="https://github.com/rikyoz/bit7z">bit7z</a>.

The project also needs 7-Zip DLL to extract the data from downloaded update files.

## Installation
### Windows
If you don't want to compile the program by yourself, you can head over the <a href="https://github.com/phuongtran7/Trello2Monthly/releases">releases</a> tab a get a pre-compiled version.

1. Install cpprestsdk, spdlog and cpptoml with Microsoft's <a href="https://github.com/Microsoft/vcpkg">vcpkg</a>.
2. Clone the project: `git clone https://github.com/phuongtran7/Trello2Monthly.git`.
3. Build the project.

### Linux
Coming soon.

## Preparing Trello
Trello2Monthly expects a certain way of using Trello to successfully generate a Monthly Status. There are four important components in Trello: `Board`, `List`, `Card` and `Label`.

Trello2Monthly expects a `Board` to represent the Month for the status report. Everything should be included in a Board for that month. The `Board` name should contain the month and year for the report. For example `Monthly Status Report - August 1997`. 

A `Board` can contain many `Lists`, each of these `Lists` represents a week in the month. The name of the `List` should be something like `8/24/1997 -- 8/30/1997` for easy integration with the LaTEX template.

In the `List`, there can be as many `Cards` as required. These `Cards` represents the task that is completed in that week. Each of the `Cards`(Tasks) can also use the `Description` box to explain more about that particular task. The `Description` will appear as a subitem under the `Card` name in the generated PFD file.

Due to the nature of this work, there are many projects can run parallel with each other, therefore the `Labels` are used to mark which task belongs to which projects. A task can have multiple `Labels` and it will appear at multiple sections in the final PDF file. 

There are two ways to report the work hours for each week. The first one is to use `Label`. Create a new `Label` called `Hour Breakdown` and then tag the cards that contain the work hour. For example: a `Card` named `Boeing 737: 10 Hours` with the tag `Hour Breakdown` will be put into the Hour Breakdown section of the PDF file. There can be as many `Cards` that tagged with `Hour Breakdown` as needed. Each of the `Cards` can represent a different project for that week.

The second way of reporting the hours is to manually specify the work hour on each of the tasks (`Cards`). To achieve this, we have to take advantage of the Power-Ups in Trello. Each of free user can have one Power-Up per board and we can use it to install `Custom Field Power-Ups`. This Power-Up allow the user to create a filed inside a `Card` and attach a value to each. In this case, we can create a `Custom Field` named `Work Hour` for the board. Then in each of the `Cards` in the week, the user can add the work hour for that particular `Card`. When generate the report, the program will combine all work hours of the `Cards` that have the same `Label`. For example, there are two `Cards` represent two complete tasks, these two `Cards` have the same label `Boeing 737`. The first `Card` has the custom field `Work Hour` with value of 6 and second one has the value of 4. When compiling the report, the program will add these two together and put in the `Hour Breakdown` section `Boeing 737: 10 Hours`. This allows the user to have a more fine-grained control over the task work hour.

The user can mix the two ways of reporting hour in a same `Board`, however, the user should commit to use only one way for the `List`. For example, in a `Board` there are four `Lists`, represent four weeks in that month. The user can use the general reporting hour by using `Label` for three of the `Lists` and using the more fine-grained control for the last one.

## Usage
1. Install <a href="https://miktex.org/">MiKTeX</a> and update it.
2. (Optional) Install <a href="https://pandoc.org/installing.html">Pandoc</a> if Word fil is needed. 
3. Get API Key and Token from <a href="https://developers.trello.com/docs/api-introduction">Trello</a>.
4. Prepare a `config.toml` file with content:
```
[Configuration]
key = "DEVELOPER KEY"
token = "TOKEN"
author = "AUTHOR NAME"
```
and then put it next to the compiled executable.
4. Download <a href="https://www.7-zip.org/download.html">7-Zip</a> and copy `7z.dll` into executable's folder.
5. Start the executable.