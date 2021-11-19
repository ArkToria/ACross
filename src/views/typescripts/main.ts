
function toggleVisibilty(component: any): void {
    if (component.visible === false) {
        component.show()
    } else {
        component.hide()
    }
}
