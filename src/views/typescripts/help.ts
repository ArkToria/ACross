function getLogo(iconStyle: string = "", controlItem: any = null, svgBox: any = null): string {
    let logoPath: string
    let style: string = ""

    if (controlItem === null && iconStyle != "") {
        style = iconStyle
    } else {
        style = controlItem.isDev ? "dev" : iconStyle
        controlItem.isDev = !controlItem.isDev
    }

    switch (style) {
        case "dev":
            logoPath = "qrc:/misc/design/lines.svg"
            break
        case "light":
            logoPath = "qrc:/misc/design/logo_pure.svg"
            break
        default:
            logoPath = "qrc:/misc/design/logo.svg"
            break
    }

    if (svgBox != null) {
        svgBox.source = logoPath
    }


    return logoPath
}
