# Trello2Monthly
<h4 align="center">A command line program to build Monthly Status Report.</h4>

Trello2Monthly is a command line program to generate PDF and Word files from Trello. Trello2Monthly uses <a href="https://github.com/Microsoft/cpprestsdk">cpprestsdk</a> and <a href="https://github.com/gabime/spdlog">spdlog</a> and <a href="https://github.com/skystrife/cpptoml">cpptoml</a>.

## Installation
1. Install cpprestsdk and spdlog with Microsoft's <a href="https://github.com/Microsoft/vcpkg">vcpkg</a>.
2. Clone the project: `git clone https://github.com/phuongtran7/Trello2Monthly.git`.
3. Build the project.

## Preparing Trello
Trello2Monthly expects a certain way of using Trello to successfully generate a Monthly Status. There are four important components in Trello: Board, List, Card and Label.

Trello2Monthly expects a `Board` to represent the Month for the status report. Everything should be included in a Board for that month. The `Board` name should be the month of that report. For example `August 1997`.

A `Board` can contain many `Lists`, each of these `Lists` represents a week in the month. The name of the `List` should be something like `8/24/1997 -- 8/30/1997` for easy integration with the LaTEX template.

In the `List`, there can be as many `Cards` as required. Each of these `Cards` represents the task that is completed in that week.

And finally, due to the nature of this work, there are many projects can run parallel with each other, therefore the `Labels` are used to marked which task belongs to which projects. A task can have multiple `Labels` and it will appear at multiple sections in the final PDF file.

## Usage
1. Install <a href="https://miktex.org/">MiKTeX</a>
2. It's recommended to compile the Template.tex before using Trello2Monthly so that all the necessary packages are installed correctly. Otherwise, during the generation PDF process, MiKTeX will interupt and ask to install these packages.
3. Get API key and Token from <a href="https://developers.trello.com/docs/api-introduction">Trello</a>.
4. Prepare a `config.toml` file with content:
```
[Configuration]
key = "DEVELOPER-KEY"
token = "TOKEN"
author = "AUTHOR NAME"
```
and then put it next to the compiled executable.

5. Start the executable.