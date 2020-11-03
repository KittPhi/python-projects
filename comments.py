# * Open terminal and execute to install extension:
code --install-extension aaron-bond.better-comments

# * Edit User settings.json in VScode
Ctrl+Shift+p > Preference: Open Settings(JSON)

# * Paste into settings.json
"better-comments.multilineComments": true,
    "better-comments.tags": [
        {
          "tag": "!",
          "color": "#FF2D00",
          "strikethrough": false,
          "underline": false,
          "backgroundColor": "transparent",
          "bold": false,
          "italic": false
        },
        {
          "tag": "?",
          "color": "#3498DB",
          "strikethrough": false,
          "underline": false,
          "backgroundColor": "transparent",
          "bold": false,
          "italic": false
        },
        {
          "tag": "//",
          "color": "#474747",
          "strikethrough": true,
          "underline": false,
          "backgroundColor": "transparent",
          "bold": false,
          "italic": false
        },
        {
          "tag": "todo",
          "color": "#FF8C00",
          "strikethrough": false,
          "underline": false,
          "backgroundColor": "transparent",
          "bold": false,
          "italic": false
        },
        {
          "tag": "*",
          "color": "#98C379",
          "strikethrough": false,
          "underline": false,
          "backgroundColor": "transparent",
          "bold": false,
          "italic": false
        }
      ]
 # comments.py     

# Basic Comment: This code does this...
# * High Contrast Comment: This code does this...
# ? Question: This could be simpler if...
# // Strikethrough Finished and Tested...
# ! Important: Please fix this ASAP!
# *todo not bold: Task 1: Need to add error handling...
# TODO BOLD: Task 2: Need to add this to next sprint.

# * Install Comment Divider vs-code extension
code --install-extension stackbreak.comment-divider

# * type then Shift+Alt+x
# ---------------------------------------------------------------------------- #
#                                  Function 1                                  #
# ---------------------------------------------------------------------------- #

# * type then Alt+x
# -------------------------------- Function 2 -------------------------------- #

# * To customize line length
# * Set line length for all dividers.
# * Edit User settings.json in VScode
Ctrl+Shift+p > Preference: Open Settings(JSON)
  "comment-divider.length": 70,
  "comment-divider.mainHeaderFiller": "#",
    // "Set symbol for subheader line filling (only one).
  "comment-divider.subHeaderFiller": "# #",
   // Set symbol for solid line filling.
  "comment-divider.lineFiller": "#",

# * type then Shift+Alt+x
# ################################################################## #
#                                Test                                #
# ################################################################## #
# * type then Alt+x
# ----------------------------- Test 2 ----------------------------- #


# * Just FYI
code --list-extensions
code --install-extension aaron-bond.better-comments
code --uninstall-extension aaron-bond.better-comments
