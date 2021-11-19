"use strict";
function getLogo(iconStyle = "", controlItem = null, svgBox = null) {
    let logoPath;
    let style = "";
    if (controlItem === null && iconStyle != "") {
        style = iconStyle;
    }
    else {
        style = controlItem.isDev ? "dev" : iconStyle;
        controlItem.isDev = !controlItem.isDev;
    }
    switch (style) {
        case "dev":
            logoPath = "qrc:/misc/design/lines.svg";
            break;
        case "light":
            logoPath = "qrc:/misc/design/logo_pure.svg";
            break;
        default:
            logoPath = "qrc:/misc/design/logo.svg";
            break;
    }
    if (svgBox != null) {
        svgBox.source = logoPath;
    }
    return logoPath;
}
