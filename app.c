//かわロボもどき　バトルロボット

#include "ev3api.h"
#include "app.h"

#if defined (BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif
#include <string.h>

#define r_motor     EV3_PORT_A
#define l_motor     EV3_PORT_D
#define arm1     EV3_PORT_B
#define arm2     EV3_PORT_C
#define ultraSonic_sensor  EV3_PORT_4
#define color_sensor  EV3_PORT_3
#define battery

#define black   6
#define white   91


const uint32_t RUN_TIMS_MS = 1200;

int relcd=1.7;


void run_task(intptr_t unused){
    int ulso=0 ,color=0 ,bat=0;
    char c[3]={'¥0'},c2[2]={'¥0'},c3[4]={'¥0'};
    int power = -100;
    static int32_t cnt1=1000,cnt2=470;
    ev3_motor_reset_counts(l_motor);
    ev3_motor_reset_counts(r_motor);
    ev3_motor_reset_counts(arm1);
    ev3_motor_reset_counts(arm2);

    ev3_motor_rotate(arm1,cnt2,power,false);
    ev3_motor_rotate(arm2,cnt2,power,false);
    tslp_tsk(RUN_TIMS_MS);
    int ev3_battery_voltage_mV(battery);


    while(1){
    ulso=ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);
    color= ev3_color_sensor_get_reflect(color_sensor);
    bat = ev3_battery_voltage_mV(battery);

    if(ulso>=100){
        sprintf(c,"%d",ulso);
    }else{
        sprintf(c,"0%d",ulso);
    } 
    sprintf(c2,"%d",color);
    sprintf(c3,"%d",bat);

    ev3_lcd_draw_string("ultraSonic_sensor", 0,0);
    ev3_lcd_draw_string(c, 140,0);
    ev3_lcd_draw_string("cm", 160,0);

    ev3_lcd_draw_line	(0,9,300,9);
    ev3_lcd_fill_rect(0,11,ulso*relcd,11,EV3_LCD_BLACK);
    ev3_lcd_draw_line	(0,23,300,23);

    ev3_lcd_draw_string("color_sensor", 0,26);
    ev3_lcd_draw_string(c2, 140,26);

    ev3_lcd_draw_line	(0,36,300,36);
    ev3_lcd_fill_rect(0,38,color*1.79,11,EV3_LCD_BLACK);
    ev3_lcd_draw_line	(0,50,300,50);


    ev3_lcd_draw_string("Battery_Voltage", 0,53);
    ev3_lcd_draw_string(c3, 130,53);
    ev3_lcd_draw_string("mV", 160,53);

    ev3_lcd_draw_line	(0,63,300,63);
    ev3_lcd_fill_rect(0,65,bat*0.018,11,EV3_LCD_BLACK);
    ev3_lcd_draw_line	(0,77,300,77);

//move

    if(color<=20){
        ev3_motor_stop(l_motor,true);
        ev3_motor_stop(r_motor,true);
        ev3_motor_steer(l_motor,r_motor,-power,0);
        tslp_tsk(300);
        ev3_motor_steer(l_motor,r_motor,-power,100);
        tslp_tsk(RUN_TIMS_MS);
    }else{
        ev3_motor_steer(l_motor,r_motor,power,0);
    }
//

    // ev3_motor_steer(l_motor,r_motor,power,0);

    // ev3_motor_stop(l_motor,true);
    // ev3_motor_stop(r_motor,true);
    // ev3_motor_stop(arm1,true);
    // ev3_motor_stop(arm2,true);
    // tslp_tsk(WAIT_TIMS_MS);

    // cnt1=ev3_motor_get_counts(l_motor)*-1;
    // ev3_motor_rotate(l_motor,cnt1,power,false);
    // ev3_motor_rotate(r_motor,cnt1,power,false); 

//attack
    if(ulso<=6.7){
    ev3_motor_steer(l_motor,r_motor,power*0.07,0);
    ev3_motor_rotate(arm1,cnt2,-power,false);
    ev3_motor_rotate(arm2,cnt2,-power,true); 
    tslp_tsk(400);
    ev3_motor_stop(l_motor,true);
    ev3_motor_stop(r_motor,true);
    ev3_motor_rotate(arm1,cnt2,power,false);
    ev3_motor_rotate(arm2,cnt2,power,true);
    }
//

    // tslp_tsk(RUN_TIMS_MS);
    // ev3_motor_stop(arm1,true);
    // ev3_motor_stop(arm2,true);






    ev3_lcd_fill_rect(ulso*relcd,11,300,11,EV3_LCD_WHITE);
    ev3_lcd_fill_rect(color*1.79,38,300,11,EV3_LCD_WHITE);
    ev3_lcd_fill_rect(bat*0.018,65,300,11,EV3_LCD_WHITE);
    //ev3_lcd_fill_rect(140,0,100,8,EV3_LCD_WHITE);
    }
}

void main_task(intptr_t unused){
    ev3_motor_config(l_motor,LARGE_MOTOR);
    ev3_motor_config(r_motor,LARGE_MOTOR);
    ev3_motor_config(arm1,LARGE_MOTOR);
    ev3_motor_config(arm2,LARGE_MOTOR);
    ev3_sensor_config(ultraSonic_sensor, ULTRASONIC_SENSOR);
    ev3_sensor_config(color_sensor, COLOR_SENSOR);

    act_tsk(RUN_TASK);
}
