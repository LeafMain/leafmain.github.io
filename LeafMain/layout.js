(function () {
    "use strict";
    const layout = {
        elements: {
            header: document.getElementById("app_header"),
            main: document.getElementById("app_main"),
            footer: document.getElementById("app_footer"),
            checkExistence() {
                if (!this.header || !this.main || !this.footer) {
                    const body = document.body || document.documentElement;
                    const p = document.createElement("p");
                    p.id = "app_layout_missing_error";
                    p.style.textAlign = "center";
                    p.textContent = "使用 app_layout 基本布局缺少必要的 id 元素" +
                        "app_header、app_main、app_footer" +
                        "，请检查。";
                    body.prepend(p);
                }
            },
        },
        header: {
            elements: {
                detailsMoreTools: document.getElementById("details_more_tools"),
                detailsSwitchMode: document.getElementById("details_switch_mode"),
                switchModeDialog: document.getElementById("switch_mode_dialog"),
                switchModeConfirm: document.getElementById("switch_mode_confirm"),
                pageDefaultMode: document.getElementById("page_default_mode"),
                pageLightMode: document.getElementById("page_light_mode"),
                pageDarkMode: document.getElementById("page_dark_mode"),
            },
            /**
             * @description 初始化头部
             */
            init() {
                this.bindEvents();
            },
            /**
             * @description 绑定头部事件
             * @param {Event} event
             */
            bindEvents() {
                // 切换模式按钮事件
                this.elements.detailsSwitchMode.addEventListener("click", (event) => {
                    event.stopPropagation();
                    this.showSwitchModeDialog();
                });
                // 切换模式对话框遮罩层点击事件
                this.elements.switchModeDialog.addEventListener("click", (event) => {
                    const dialog = this.elements.switchModeDialog;
                    event.stopPropagation();
                    if (dialog === event.target) {
                        dialog.close();
                    }
                })
                // 切换模式确认按钮事件
                this.elements.switchModeConfirm.addEventListener("click", () => {
                    this.closeSwitchModeDialog();
                });
                // 点击其他地方关闭详情面板
                window.addEventListener("click", (event) => {
                    if (!this.elements.detailsMoreTools.contains(event.target)) {
                        this.closeMoreToolDatails();
                    }
                });
                // 设置页面模式
                this.elements.switchModeDialog.addEventListener("close", () => {
                    if (this.elements.pageLightMode.checked) {
                        this.setPageMode("light");
                    } else if (this.elements.pageDarkMode.checked) {
                        this.setPageMode("dark");
                    } else if (this.elements.pageDefaultMode.checked) {
                        this.setPageMode("default");
                    }
                });
                // 应用当前页面模式
                window.addEventListener("DOMContentLoaded", () => {
                    this.applyPageMode();
                });
            },
            /**
             * @description 关闭详情面板
             */
            closeMoreToolDatails() {
                const details = this.elements.detailsMoreTools;
                if (details.open) {
                    details.open = false;
                }
            },
            /**
             * @description 显示切换模式对话框
             */
            showSwitchModeDialog() {
                const dialog = this.elements.switchModeDialog;
                const details = this.elements.detailsMoreTools;
                if (details.open) {
                    details.open = false;
                }
                if (dialog.open) {
                    dialog.close();
                } else {
                    dialog.showModal();
                }
            },
            /**
             * @description 关闭切换模式对话框
             */
            closeSwitchModeDialog() {
                const dialog = this.elements.switchModeDialog;
                if (dialog.open) {
                    dialog.close();
                }
            },
            /**
             * @description 设置页面模式
             * @param {string} mode 页面模式
             * 可选值：light、dark、default
             */
            setPageMode(mode) {
                const lightMode = this.elements.pageLightMode;
                const darkMode = this.elements.pageDarkMode;
                const defaultMode = this.elements.pageDefaultMode;
                const page = document.documentElement;
                switch (mode) {
                    case "light":
                        page.classList.add("light-mode");
                        page.classList.remove("dark-mode");
                        page.classList.remove("default-mode");
                        lightMode.checked = true;
                        darkMode.checked = false;
                        defaultMode.checked = false;
                        break;
                    case "dark":
                        page.classList.add("dark-mode");
                        page.classList.remove("light-mode");
                        page.classList.remove("default-mode");
                        lightMode.checked = false;
                        darkMode.checked = true;
                        defaultMode.checked = false;
                        break;
                    default:
                        page.classList.add("default-mode");
                        page.classList.remove("light-mode");
                        page.classList.remove("dark-mode");
                        lightMode.checked = false;
                        darkMode.checked = false;
                        defaultMode.checked = true;
                        break;
                }
                // 防止切换的一瞬间过渡导致闪屏
                if (mode) {
                    if (typeof window.localStorage !== "undefined") {
                        localStorage.setItem("layoutPageMode", mode);
                    }
                    page.classList.add("no-transition");
                    setTimeout(() => {
                        page.classList.remove("no-transition");
                    }, 88);
                }
            },
            /**
             * @description 应用当前页面模式
             */
            applyPageMode() {
                if (typeof window.localStorage !== "undefined") {
                    const mode = localStorage.getItem("layoutPageMode");
                    if (mode) {
                        this.setPageMode(mode);
                    }
                }
            }
        },
        main: {
            elements: {

            },
        },
        footer: {
            elements: {

            },
        },
        init() {
            this.elements.checkExistence();
            this.header.init();
            // this.header.applyPageMode();
        }
    };
    layout.init();
})();