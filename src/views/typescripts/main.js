"use strict";
function toggleVisibilty(component) {
    if (component.visible === false) {
        component.show();
    }
    else {
        component.hide();
    }
}
