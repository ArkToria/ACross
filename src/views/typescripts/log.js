"use strict";
function ensureVisible(flick, r) {
    if (flick.contentX >= r.x)
        flick.contentX = r.x;
    else if (flick.contentX + flick.width <= r.x + r.width)
        flick.contentX = r.x + r.width - flick.width;
    if (flick.contentY >= r.y)
        flick.contentY = r.y;
    else if (flick.contentY + flick.height <= r.y + r.height)
        flick.contentY = r.y + r.height - flick.height;
}
function autoClean(textEdit, maxLines = 10) {
    if (maxLines <= 0) {
        // unlimit history lines
    }
    else if (textEdit.lineCount >= maxLines) {
        textEdit.clear();
    }
}
