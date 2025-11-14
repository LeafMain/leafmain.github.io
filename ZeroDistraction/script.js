(function ZeroDistraction() {
    "use strict";
    // 本地数据
    let data = {
        userBookAgreed: false, // 用户协议是否同意
        currentFocusTime: 0,
        accumulatedFocusTime: 0,
        logInTime: 0, // 登录时间
        logOutTime: 0, // 退出时间
        validClose: true, // 是否正常关闭应用
    };
    // 本地存储
    const storage = {
        key: "ZeroDistraction",
        // 保存本地数据
        save() {
            localStorage.setItem(this.key, JSON.stringify(data));
            console.info("本地数据已保存");
        },
        // 读取本地数据
        load() {
            const savedData = localStorage.getItem(this.key);
            if (savedData) {
                data = JSON.parse(savedData);
            }
            console.info("本地数据已读取");
        },
        clear() {
            localStorage.removeItem(this.key);
            console.info("本地数据已清除");
        }
    };
    // 应用数据和方法
    const app = {
        // 应用初始化
        initialize() {
            // storage.clear(); // 暂时清除本地数据，以便测试
            storage.load();
            this.pageCover.bindEvents();
            this.pageCover.enter();
            this.pageMain.bindEvents();
            this.pageMain.updateData();
            this.dialogs.bindEvents();
        },
        // 封面页面
        pageCover: {
            element: {
                self: document.getElementById("page_cover"),
                userBookAgree: document.getElementById("user_book_agree"),
                userBookLink: document.getElementById("user_book_link"),
                enterMainButton: document.getElementById("enter_main_button"),
            },
            // 封面页面是否已进入
            entered: false,
            // 进入封面页面
            enter() {
                app.pageCover.element.self.style.display = "flex";
                app.pageMain.element.self.style.display = "none";
                app.pageCover.entered = true;
                app.pageMain.entered = false;
                if (data.userBookAgreed) {
                    this.element.userBookAgree.checked = true;
                }
            },
            // 绑定事件
            bindEvents() {
                // 绑定同意协议点击事件
                this.element.userBookAgree.addEventListener("change", () => {
                    data.userBookAgreed = this.element.userBookAgree.checked;
                });
                // 绑定开始按钮点击事件
                this.element.enterMainButton.addEventListener("click", () => {
                    if (data.userBookAgreed) {
                        app.pageMain.enter();
                    } else {
                        this.showUserBook();
                    }
                });
                // 停用右键菜单
                document.addEventListener("contextmenu", (event) => {
                    if (!this.entered) return;
                    event.preventDefault();
                });
                // 停用 F11 和 F12 快捷键
                document.addEventListener("keydown", (event) => {
                    if (!this.entered) return;
                    if (event.key === "F12" || event.key === "F11") {
                        event.preventDefault();
                    }
                });
                // 页面可见性变化时保存本地数据
                document.addEventListener("visibilitychange", () => {
                    if (!document.hidden) {
                        storage.save();
                    }
                });
                // 添加登录时间
                window.addEventListener("load", () => {
                    data.logInTime = Date.now();
                });
                // 添加退出事件
                window.addEventListener("unload", () => {
                    data.logOutTime = Date.now();
                    storage.save();
                });
            },
            showUserBook() {

            },
        },
        // 主页面
        pageMain: {
            element: {
                self: document.getElementById("page_main"),
                systemDatetime: document.getElementById("system_datetime"),
                systemTime: document.getElementById("system_time"),
                systemBatteryBar: document.getElementById("system_battery_bar"),
                systemBatteryLevel: document.getElementById("system_battery_level"),
                currentFocusTime: document.getElementById("current_focus_time"),
                accumulatedFocusTime: document.getElementById("accumulated_focus_time"),
                focusInfo: document.getElementById("focus_info"),
                exitFocusButton: document.getElementById("exit_focus_button"),
            },
            // 主页面是否已进入
            entered: false,
            // 进入主页面
            enter() {
                app.pageMain.element.self.style.display = "flex";
                app.pageCover.element.self.style.display = "none";
                app.pageMain.entered = true;
                app.pageCover.entered = false;
                this.startFocus();
                // if (!data.validClose) {
                //     this.regardInvalid();
                // }
            },
            // 计时器及状态
            timer: {
                datetime: null,
                time: null,
                focus: null,
                delay: null,
                focusPaused: false,
                wakeLock: null,
            },
            // 惩罚值
            punish: {
                // 时间点
                timepoint: {
                    pageHide: 0,
                    pageShow: 0,
                    pageFocus: 0,
                    pageBlur: 0,
                },
                // 重置惩罚值
                reset() {
                    this.pointerDown.value = this.pointerDown.initial;
                    this.mouseMove.value = this.mouseMove.initial;
                    this.keyDown.value = this.keyDown.initial;
                    this.uncharge.value = this.uncharge.initial;
                    this.deviceMotion.value = this.deviceMotion.initial;
                    this.hesitation.value = this.hesitation.initial;
                    this.beforeUnload.value = this.beforeUnload.initial;
                    this.resize.value = this.resize.initial;
                    this.blur.value = this.blur.initial;
                    data.currentFocusTime = 0;
                },
                // 当按下鼠标时，惩罚一定的专注时间
                pointerDown: {
                    value: 4,
                    initial: 4,
                    timer: null,
                    increment: 4,
                    max: 300,
                    interval: 1000,
                },
                // 当鼠标移动时，惩罚一定的专注时间
                mouseMove: {
                    value: 2,
                    initial: 2,
                    timer: null,
                    increment: 1,
                    interval: 500,
                },
                // 当按下键盘时，惩罚一定的专注时间
                keyDown: {
                    value: 4,
                    initial: 4,
                    timer: null,
                    increment: 4,
                    interval: 1000,
                },
                // 当断开电源时，惩罚一定的专注时间
                uncharge: {
                    value: 15,
                    initial: 15,
                    timer: null,
                    increment: 15,
                    interval: 2000,
                },
                // 当设备运动时，惩罚一定的专注时间
                deviceMotion: {
                    value: 2,
                    initial: 2,
                    timer: null,
                    increment: 1,
                    interval: 500,
                },
                // 当犹豫地退出专注时，惩罚一定的专注时间
                hesitation: {
                    value: 10,
                    initial: 15,
                    timer: null,
                    increment: 0,
                    interval: 2000,
                },
                // 当即将关闭页面时，惩罚一定的专注时间
                beforeUnload: {
                    value: 30,
                    initial: 30,
                    timer: null,
                    increment: 15,
                    interval: 3000,
                },
                // 当窗口大小调整时，惩罚一定的专注时间
                resize: {
                    value: 5,
                    initial: 5,
                    timer: null,
                    increment: 5,
                    interval: 1000,
                },
                // 页面失去焦点时，惩罚一定的专注时间
                blur: {
                    value: 10,
                    initial: 30,
                    timer: null,
                    increment: 10,
                    interval: 1500,
                },
                // 当页面隐藏时，惩罚一定的专注时间
                visibilityChange: {
                    times: 4,
                    timer: null,
                    interval: 2000,
                },
            },
            // 设备运动传感器
            motion: {
                PRECISION: 2.0,
                lastX: 0,
                lastY: 0,
                lastZ: 0,
                x: 0,
                y: 0,
                z: 0,
                // 设置数据
                setData(x, y, z) {
                    this.lastX = this.x;
                    this.lastY = this.y;
                    this.lastZ = this.z;
                    this.x = x;
                    this.y = y;
                    this.z = z;
                },
                // 数据是否发生变化
                changed() {
                    return Math.abs(this.x - this.lastX) > this.PRECISION ||
                        Math.abs(this.y - this.lastY) > this.PRECISION ||
                        Math.abs(this.z - this.lastZ) > this.PRECISION;
                }
            },
            // 绑定事件
            bindEvents() {
                // 绑定退出专注按钮事件
                this.element.exitFocusButton.addEventListener("click", () => {
                    app.pageMain.requestExitFocus();
                });
                // 添加窗口鼠标指针按下事件
                this.element.self.addEventListener("pointerdown", (event) => {
                    if (event.target === this.element.exitFocusButton) return;
                    if (data.currentFocusTime < 1) return;
                    this.punishPointerDown();
                });
                // 添加页面鼠标移动事件
                this.element.self.addEventListener("mousemove", (event) => {
                    if (data.currentFocusTime < 1) return;
                    if (event.target === this.element.exitFocusButton) return;
                    this.punishMouseMove();
                });
                // 停用页面右键菜单
                document.addEventListener("contextmenu", (event) => {
                    if (!this.entered) return;
                    event.preventDefault();
                });
                // 添加页面可见性变化事件
                document.addEventListener("visibilitychange", () => {
                    if (!this.entered) return;
                    if (document.hidden) {
                        this.punish.timepoint.pageHide = Date.now();
                    } else {
                        this.punish.timepoint.pageShow = Date.now();
                        this.punishVisibilityChange();
                    }
                });
                // 添加全屏模式事件
                document.addEventListener("fullscreenchange", () => {
                    if (!this.entered) return;
                    if (!document.fullscreenElement) {
                        this.findExitFullscreen();
                    }
                });
                // 添加窗口失去焦点事件
                window.addEventListener("blur", () => {
                    if (!this.entered) return;
                    this.punishBlur();
                });
                // 添加窗口键盘按下事件
                window.addEventListener("keydown", (event) => {
                    if (!this.entered) return;
                    if (event.key === "Escape") return;
                    if (event.repeat) return;
                    if (app.dialogs.exitFocus.isShow) {
                        if (event.key.length > 1 &&
                            event.key != "Backspace") {
                            event.preventDefault();
                        }
                        return;
                    }
                    event.preventDefault();
                    if (event.shiftKey) {
                        if (event.key.toUpperCase() === "E") {
                            this.element.exitFocusButton.click();
                        }
                    }
                    else {
                        if (data.currentFocusTime < 1) return;
                        this.punishKeyDown();
                    }
                });
                // 添加窗口即将退出事件
                window.addEventListener("beforeunload", (event) => {
                    if (!this.entered) return;
                    event.preventDefault();
                    this.punishBeforeUnload();
                });
                // 添加窗口大小调整事件
                window.addEventListener("resize", () => {
                    if (!this.entered) return;
                    this.punishResize();
                });
                // 绑定设备运动事件，需支持且不要求权限
                if (DeviceMotionEvent &&
                    !DeviceMotionEvent.requestPermission) {
                    window.addEventListener("devicemotion", (event) => {
                        if (!this.entered) return;
                        const ac = event.accelerationIncludingGravity;
                        this.motion.setData(ac.x, ac.y, ac.z);
                        if (this.motion.changed()) {
                            this.punishDeviceMotion();
                            // 将数据更新为当前值，防止重复触发
                            this.motion.setData(ac.x, ac.y, ac.z);
                        }
                    });
                }
            },
            // 更新数据
            updateData() {
                this.updateDatetime();
                this.updateBattery();
                clearInterval(this.timer.datetime);
                this.timer.datetime = setInterval(() => {
                    this.updateDatetime();
                }, 250);
            },
            // 更新系统时间
            updateDatetime() {
                const now = new Date();
                const date = now.toLocaleDateString();
                const time = now.toLocaleTimeString();
                this.element.systemDatetime.innerText = date;
                this.element.systemTime.innerText = time;
            },
            // 更新电池信息
            updateBattery() {
                const update = (battery) => {
                    const level = this.element.systemBatteryLevel;
                    this.element.systemBatteryBar.value = battery.level;
                    if (battery.charging) {
                        level.style.fontWeight = "bold";
                        level.innerText = `${battery.level * 100}% +`
                    } else {
                        level.style.fontWeight = "normal";
                        level.innerText = `${battery.level * 100}%`
                    }
                };
                const battery = navigator.getBattery();
                battery.then((battery) => {
                    update(battery);
                    battery.addEventListener("levelchange", () => {
                        update(battery);
                        // 电量低于 10% 时，退出专注
                        if (battery.level < 0.1 && this.entered) {
                            this.exitDueToLowBattery();
                        }
                    });
                    battery.addEventListener("chargingchange", () => {
                        update(battery);
                        if (this.entered && !battery.charging) {
                            this.punishUncharge();
                        }
                    });
                });
            },
            // 改变专注时间，正数增加，负数减少
            altFocus(second = 0) {
                if (second < 0) console.log("减少专注时间" + -second + "秒");
                const MAX_FOCUS = 360000;
                const MAX_ACC = 3600000000;
                data.accumulatedFocusTime += second;
                data.currentFocusTime += second;
                // 如果数值小于 0， 则置为 0
                if (data.currentFocusTime < 0) {
                    data.currentFocusTime = 0;
                }
                if (data.accumulatedFocusTime < 0) {
                    data.accumulatedFocusTime = 0;
                }
                // 如果数值大于最大专注时间， 则置为最大专注时间
                if (data.currentFocusTime > MAX_FOCUS) {
                    data.currentFocusTime = MAX_FOCUS;
                }
                if (data.accumulatedFocusTime > MAX_ACC) {
                    data.accumulatedFocusTime = 0;
                }
                this.updateFocus();
            },
            // 更新专注信息
            updateFocus() {
                const toTime = (time) => {
                    let hour = String(Math.floor(time / 3600))
                        .padStart(2, "0");
                    let minute = String(Math.floor(time / 60) % 60)
                        .padStart(2, "0");
                    let second = String(time % 60).padStart(2, "0");
                    return `${hour}:${minute}:${second}`;
                };
                this.element.currentFocusTime.innerText =
                    toTime(data.currentFocusTime);
                this.element.accumulatedFocusTime.innerText =
                    toTime(data.accumulatedFocusTime);
            },
            // 开始专注
            startFocus() {
                data.validClose = false;
                // 重置专注
                this.resetFocus();
                // 开始计时
                clearInterval(this.timer.focus);
                this.timer.focus = setInterval(() => {
                    if (this.timer.focusPaused) {
                        return;
                    };
                    this.altFocus(1);
                    this.updateFocus();
                }, 1000);
                // 请求屏幕唤醒
                this.timer.wakeLock = navigator.wakeLock.request("screen");
                // 请求指针锁定
                if (this.element.self.requestPointerLock) {
                    this.element.self.requestPointerLock();
                }
                // 进入全屏模式
                if (document.fullscreenEnabled) {
                    document.body.requestFullscreen();
                }
            },
            // 延迟专注
            delayFocus(delay = 1000) {
                this.timer.focusPaused = true;
                clearTimeout(this.timer.delay);
                this.timer.delay = setTimeout(() => {
                    this.timer.focusPaused = false;
                }, delay);
            },
            // 暂停专注，pause 为 true 时暂停，false 时继续
            pauseFocus(pause = true) {
                this.timer.focusPaused = pause;
            },
            // 直接退出专注
            exitFocus() {
                data.validClose = true;
                this.timer.focusPaused = false;
                clearInterval(this.timer.focus);
                app.pageCover.enter();
                // 释放屏幕唤醒
                this.timer.wakeLock.then((wakeLock) => {
                    wakeLock.release();
                });
                // 释放指针锁
                if (document.pointerLockElement) {
                    document.exitPointerLock();
                }
                // 退出全屏模式
                if (document.fullscreenElement) {
                    document.exitFullscreen();
                }
            },
            // 重置专注
            resetFocus() {
                data.currentFocusTime = 0;
                this.punish.reset();
                this.updateFocus();
            },
            // 认为专注时间无效
            regardInvalid() {
                data.accumulatedFocusTime -= data.currentFocusTime;
                if (data.accumulatedFocusTime < 0) {
                    data.accumulatedFocusTime = 0;
                }
                data.currentFocusTime = 0;
                this.updateFocus();
            },
            // 请求退出专注
            requestExitFocus() {
                if (document.fullscreenElement) {
                    app.dialogs.exitFocus.show();
                }
            },
            // 获取上次发送的消息
            getLastMessage() {
                return this.element.focusInfo.innerText;
            },
            // 发送消息
            sendMessage(message = "", vibrate = false) {
                const dialog = app.dialogs.distraction;
                if (message) {
                    dialog.show(message);
                } else {
                    dialog.hide();
                }
                if (vibrate && navigator.vibrate) {
                    navigator.vibrate(220);
                }
            },
            // 惩罚指针按下
            punishPointerDown() {
                const obj = this.punish.pointerDown;
                if (this.punish.pointerDown.timer) return;
                this.altFocus(-obj.value);
                this.sendMessage(
                    `你刚才点击了屏幕，分心了，已减少专注时间 ${obj.value} 秒。` +
                    `下次走神触发会多减少 ${obj.increment} 秒。`
                );
                console.info("鼠标按下罚时已触发");
                obj.value += obj.increment;
                this.punish.pointerDown.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.pointerDown.timer = null;
                }, obj.interval);
            },
            // 惩罚鼠标移动
            punishMouseMove() {
                const obj = this.punish.mouseMove;
                if (this.punish.mouseMove.timer) return;
                if (this.punish.resize.timer) return;
                this.delayFocus(obj.interval);
                this.altFocus(-obj.value);
                if (!this.getLastMessage()) {
                    this.sendMessage(
                        `你刚才移动了鼠标，减少专注时间 ${obj.value} 秒。` +
                        `请专注于当下最需要的事情，不要神游。`
                    );
                }
                console.info("鼠标移动罚时已触发");
                obj.value += obj.increment;
                this.punish.mouseMove.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.mouseMove.timer = null;
                }, obj.interval);
            },
            // 惩罚键盘按下
            punishKeyDown() {
                const obj = this.punish.keyDown;
                if (this.punish.keyDown.timer) return;
                this.altFocus(-obj.value);
                this.sendMessage(
                    `你刚才心不在焉按下了键盘，惩罚专注时间 ${obj.value} 秒。` +
                    `请保持专注做事，否则下次多减少 ${obj.increment} 秒。`
                );
                console.info("键盘按下罚时已触发");
                obj.value += obj.increment;
                this.punish.keyDown.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.keyDown.timer = null;
                }, obj.interval);
            },
            // 惩罚拔下充电线
            punishUncharge() {
                const obj = this.punish.uncharge;
                if (this.punish.uncharge.timer) return;
                this.altFocus(-obj.value);
                this.sendMessage(
                    `你拔了充电器，想玩我吗？扣你时间 ${obj.value} 秒。` +
                    `请保持专注做事，否则下次多减少 ${obj.increment} 秒。`
                );
                console.info("拔充电线罚时已触发");
                obj.value += obj.increment;
                this.punish.uncharge.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.uncharge.timer = null;
                }, obj.interval);
            },
            // 惩罚设备运动
            punishDeviceMotion() {
                const obj = this.punish.deviceMotion;
                if (this.punish.deviceMotion.timer) return;
                if (app.dialogs.exitFocus.isShow) return;
                this.altFocus(-obj.value);
                this.delayFocus(obj.interval);
                if (!this.getLastMessage()) {
                    this.sendMessage(
                        `你刚才移动了设备，是不是无法专心，想玩设备了？这样可不行，` +
                        `作为惩罚，减少专注时间 ${obj.value} 秒。请放下我，专注做事。`
                    );
                }
                console.info("设备移动罚时已触发");
                obj.value += obj.increment;
                this.punish.deviceMotion.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.deviceMotion.timer = null;
                }, obj.interval);
            },
            // 惩罚犹豫不决地退出专注
            punishHesitation() {
                const obj = this.punish.hesitation;
                if (this.punish.hesitation.timer) return;
                this.altFocus(-obj.value);
                this.sendMessage(
                    `你打开了退出弹窗却没退出，想退出专注了吗？这样可不行，` +
                    `作为惩罚，减少专注时间 ${obj.value} 秒。请专注做事。`
                );
                console.info("关闭弹窗犹豫罚时已触发");
                obj.value += obj.increment;
                this.punish.hesitation.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.hesitation.timer = null;
                }, obj.interval);
            },
            // 惩罚页面即将关闭
            punishBeforeUnload() {
                const obj = this.punish.beforeUnload;
                if (this.punish.beforeUnload.timer) return;
                this.altFocus(-obj.value);
                this.sendMessage(
                    `你刚才想要关闭页面，想退出专注了吗？如果要结束专注，请点击` +
                    `关闭按钮或按 Shift + E. 否则会扣除 ${obj.value} 秒。`
                );
                console.info("页面即将关闭罚时已触发");
                obj.value += obj.increment;
                this.punish.beforeUnload.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.beforeUnload.timer = null;
                }, obj.interval);
            },
            // 惩罚窗口大小调整
            punishResize() {
                // 如果当前专注时间小于 1 秒，则不惩罚
                if (data.currentFocusTime < 1) {
                    return;
                }
                const obj = this.punish.resize;
                if (this.punish.resize.timer) return;
                this.altFocus(-obj.value);
                this.sendMessage(
                    `你为什么调整窗口大小，想趁机使用其他应用？这样不专心，` +
                    `需要实行惩罚来作为教训，扣除专注时间 ${obj.value} 秒。`
                );
                console.info("窗口大小调整罚时已触发");
                obj.value += obj.increment;
                this.punish.resize.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.resize.timer = null;
                }, obj.interval);
            },
            // 惩罚页面失去焦点
            punishBlur() {
                const obj = this.punish.blur;
                if (this.punish.blur.timer) return;
                // 
                if (this.punish.beforeUnload.timer) return;
                this.altFocus(-obj.value);
                this.sendMessage(
                    `刚才页面离开了前台，想使用其他应用？扣你时间 ${obj.value} 秒。` +
                    `请保持页面在前台，否则下次多扣除 ${obj.increment} 秒。`
                );
                console.info("页面失去焦点罚时已触发");
                obj.value += obj.increment;
                this.punish.blur.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.blur.timer = null;
                }, obj.interval);
            },
            // 惩罚页面隐藏
            punishVisibilityChange() {
                const obj = this.punish.visibilityChange;
                if (this.punish.visibilityChange.timer) return;
                let time = Math.abs(this.punish.timepoint.pageShow -
                    this.punish.timepoint.pageHide);
                let minus = Math.floor(time * obj.times / 1000);
                this.altFocus(-minus);
                this.sendMessage(
                    `你刚才切换了页面，想使用其他应用！扣你 ${minus} 秒，秒数` +
                    `是你退出时间的 ${obj.times} 倍！保持专注，请勿切屏。`
                );
                console.info("页面隐藏罚时已触发");
                this.punish.visibilityChange.timer = setTimeout(() => {
                    this.sendMessage();
                    this.punish.visibilityChange.timer = null;
                }, obj.interval);
            },
            // 发现退出了全屏模式
            findExitFullscreen() {
                app.dialogs.distraction.hide();
                app.dialogs.exitFocus.hide();
                app.dialogs.recoverFullscreen.show();
            },
            // 因为电池电量不足而退出专注
            exitDueToLowBattery() {
                this.exitFocus();
            },
        },
        // 弹窗
        dialogs: {
            bindEvents() {
                this.exitFocus.bindEvents();
                this.recoverFullscreen.bindEvents();
            },
            // 退出专注弹窗
            exitFocus: {
                isShow: false, // 是否显示
                inputFocusTime: 0,
                element: {
                    self: document.getElementById("dialog_exit_focus"),
                    text: document.getElementById("dialog_exit_focus_text"),
                    input: document.getElementById("dialog_exit_focus_input"),
                    confirm: document.getElementById("dialog_exit_focus_confirm"),
                    cancel: document.getElementById("dialog_exit_focus_cancel"),
                },
                // 绑定事件
                bindEvents() {
                    // 绑定文字复制事件
                    this.element.text.addEventListener("copy", (event) => {
                        event.preventDefault();
                        this.element.input.value = ""
                        this.element.input.placeholder = "请自己输入内容"
                    });
                    // 绑定文字剪切事件
                    this.element.text.addEventListener("cut", (event) => {
                        event.preventDefault();
                        this.element.input.value = ""
                        this.element.input.placeholder = "请自己输入内容"
                    });
                    // 绑定输入框聚焦事件
                    this.element.input.addEventListener("focus", () => {
                        if (this.inputFocusTime === 0) {
                            this.inputFocusTime = Date.now();
                        }
                    });
                    // 输入框双击清空内容
                    this.element.input.addEventListener("dblclick", () => {
                        this.element.input.value = ""
                    });
                    // 绑定输入框粘贴事件
                    this.element.input.addEventListener("paste", (event) => {
                        event.preventDefault();
                        this.element.input.value = ""
                        this.element.input.placeholder = "请自己输入内容"
                    });
                    // 绑定确认按钮事件
                    this.element.confirm.addEventListener("click", () => {
                        if (this.check()) {
                            this.hide();
                            app.pageMain.exitFocus();
                        };
                    });
                    // 绑定取消按钮事件
                    this.element.cancel.addEventListener("click", () => {
                        this.hide();
                        app.pageMain.punishHesitation();
                    });
                },
                // 显示弹窗
                show() {
                    if (this.isShow) return;
                    this.isShow = true;
                    this.element.self.style.display = "flex";
                    this.element.input.focus();
                    app.pageMain.pauseFocus(true);
                    if (document.pointerLockElement) {
                        document.exitPointerLock();
                    }
                },
                // 隐藏弹窗
                hide() {
                    if (!this.isShow) return;
                    this.isShow = false;
                    this.element.self.style.display = "none";
                    this.element.input.blur();
                    this.inputFocusTime = 0;
                    this.element.input.value = "";
                    app.pageMain.pauseFocus(false);
                    // 如果是交互关闭的弹窗，则重置专注
                    if (!navigator.userActivation.isActive) return;
                    if (document.pointerLockElement) {
                        document.exitPointerLock();
                    }
                    if (app.pageMain.element.self.requestPointerLock) {
                        app.pageMain.element.self.requestPointerLock();
                    }
                },
                // 输入内容检查
                check() {
                    const textContent = this.element.text.textContent.trim();
                    const value = this.element.input.value.trim();
                    if (textContent === value) {
                        const diff = Date.now() - this.inputFocusTime;
                        if (diff < 6000) {
                            this.element.input.value = "";
                            this.element.input.placeholder =
                                "你输入速度过快，有可能是你在粘贴文字"
                            return false;
                        }
                        return true;
                    }
                    this.element.input.value = "";
                    this.element.input.placeholder = "输入内容有误，请重新输入"
                    return false;
                },
            },
            
            // 恢复全屏模式弹窗
            recoverFullscreen: {
                isShow: false, // 是否显示
                element: {
                    self: document.getElementById("dialog_recover_fullscreen"),
                    confirm: document.getElementById(
                        "dialog_recover_fullscreen_confirm"
                    ),
                },
                // 绑定事件
                bindEvents() {
                    this.element.confirm.addEventListener("click", (event) => {
                        event.stopPropagation();
                        this.hide();
                        this.recover();
                    });
                },
                // 显示弹窗
                show() {
                    if (this.isShow) return;
                    app.pageMain.pauseFocus(true);
                    this.element.self.style.display = "flex";
                    this.isShow = true;
                },
                // 隐藏弹窗
                hide() {
                    if (!this.isShow) return;
                    app.pageMain.pauseFocus(false);
                    this.element.self.style.display = "none";
                    this.isShow = false;
                },
                // 恢复全屏模式
                recover() {
                    // 恢复指针锁定
                    if (!document.pointerLockElement) {
                        app.pageMain.element.self.requestPointerLock();
                    }
                    // 恢复全屏模式
                    if (document.body.requestFullscreen) {
                        document.body.requestFullscreen();
                    }
                    app.pageMain.timer.wakeLock = 
                        navigator.wakeLock.request("screen");
                },
            },
            // 分心弹窗
            distraction: {
                isShow: false, // 是否显示
                element: {
                    self: document.getElementById("dialog_distraction"),
                    text: document.getElementById("dialog_distraction_text"),
                },
                // 显示弹窗
                show(text = "") {
                    this.element.self.style.display = "flex";
                    this.element.text.innerText = text;
                    this.isShow = true;
                },
                // 隐藏弹窗
                hide() {
                    this.element.self.style.display = "none";
                    this.element.text.innerText = "";
                    this.isShow = false;
                },
            },
        }
    };
    // 应用初始化
    app.initialize();
})();