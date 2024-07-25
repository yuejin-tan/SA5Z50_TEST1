/*
 * guiInit.c
 *
 *  Created on: 2021年4月20日
 *      Author: yuejinTan
 */

#include "guiInit.h"

#include "lvgl/lvgl.h"
#include "lvgl/porting/lv_port_disp.h"
#include "lvgl/porting/lv_port_indev.h"

#include "STAR.h"
#include "STAR_gpio.h"

static void switch1_event_handler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        if (lv_switch_get_state(obj))
        {
            GPIO_ResetBit(STAR_GPIO0, GPIO_Pin_23);
        }
        else
        {
            GPIO_SetBit(STAR_GPIO0, GPIO_Pin_23);
        }
    }
}
static void switch2_event_handler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        if (lv_switch_get_state(obj))
        {
            GPIO_ResetBit(STAR_GPIO0, GPIO_Pin_22);
        }
        else
        {
            GPIO_SetBit(STAR_GPIO0, GPIO_Pin_22);
        }
    }
}

static lv_obj_t* switch1;
static lv_obj_t* switch2;
static void switch_set(void)
{
    switch1 = lv_switch_create(lv_scr_act(), NULL);
    lv_obj_set_size(switch1, 150, 50);
    lv_obj_set_pos(switch1, 60, 50);
    lv_obj_set_style_local_value_str(switch1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "D3");
    lv_obj_set_style_local_value_align(switch1, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_MID);
    // lv_obj_set_style_local_bg_opa(switch1, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_OPA_40);
    // lv_obj_set_style_local_bg_opa(switch1, LV_SWITCH_PART_KNOB, LV_STATE_DEFAULT, LV_OPA_60);
    // lv_obj_set_style_local_bg_opa(switch1, LV_SWITCH_PART_INDIC, LV_STATE_DEFAULT, LV_OPA_60);
    // lv_obj_set_style_local_outline_opa(switch1, LV_SWITCH_PART_BG, LV_STATE_FOCUSED, LV_OPA_60);
    // lv_obj_set_style_local_value_opa(switch1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_60);
    lv_obj_set_style_local_value_color(switch1, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    switch2 = lv_switch_create(lv_scr_act(), switch1);
    lv_obj_set_x(switch2, 270);
    lv_obj_set_style_local_value_str(switch2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, "D2");

    lv_obj_set_event_cb(switch1, switch1_event_handler);
    lv_obj_set_event_cb(switch2, switch2_event_handler);
}

static lv_obj_t* taskBar;
static lv_obj_t* taskBarLableUp;
static lv_obj_t* taskBarLableMid;
static lv_obj_t* taskBarLableDown;

void infoBar_task(lv_task_t* task)
{
    (void)task;
    lv_label_set_text_fmt(taskBarLableUp, "LVGL V7.11 TEST", millisCnt);
    lv_obj_realign(taskBarLableUp);
    lv_label_set_text_fmt(taskBarLableMid, "millis:%8x", millisCnt);
    lv_obj_realign(taskBarLableMid);
    lv_label_set_text_fmt(taskBarLableDown, "@SA5Z50 CM33-FPGA", millisCnt);
    lv_obj_realign(taskBarLableDown);
}

static void infoBar_set(void)
{
    const char* initStr = "GUI initing...";

    taskBar = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_pos(taskBar, 0, 200);
    lv_obj_set_size(taskBar, 480, 120);
    lv_obj_clean_style_list(taskBar, LV_OBJ_PART_MAIN);
    lv_obj_set_style_local_bg_color(taskBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    // lv_obj_set_style_local_bg_opa(taskBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_60);

    taskBarLableUp = lv_label_create(taskBar, NULL);
    lv_obj_set_style_local_text_color(taskBarLableUp, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    // lv_obj_set_style_local_text_opa(taskBarLableUp, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_60);

    taskBarLableMid = lv_label_create(taskBar, taskBarLableUp);
    taskBarLableDown = lv_label_create(taskBar, taskBarLableUp);

    lv_obj_align(taskBarLableUp, taskBar, LV_ALIGN_IN_TOP_MID, 10, 0);
    lv_label_set_align(taskBarLableUp, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text_fmt(taskBarLableUp, initStr);
    lv_obj_realign(taskBarLableUp);

    lv_obj_align(taskBarLableMid, taskBar, LV_ALIGN_CENTER, 10, 0);
    lv_label_set_align(taskBarLableMid, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text_fmt(taskBarLableMid, initStr);
    lv_obj_realign(taskBarLableMid);

    lv_obj_align(taskBarLableDown, taskBar, LV_ALIGN_IN_BOTTOM_MID, 10, 0);
    lv_label_set_align(taskBarLableDown, LV_LABEL_ALIGN_CENTER);
    lv_label_set_text_fmt(taskBarLableDown, initStr);
    lv_obj_realign(taskBarLableDown);

    lv_task_t* task_handle = lv_task_create(infoBar_task, 250, LV_TASK_PRIO_LOW, NULL);
    (void)task_handle;
}

void guiInit(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);

    switch_set();
    infoBar_set();
}

void guiLoopMain(void)
{
    lv_task_handler();
}
