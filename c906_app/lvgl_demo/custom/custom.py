is_increse = 1
is_up = 1
speedNum = 0
spd_chart = []
for i in range(100):
    spd_chart.append(0)

def screen_meter_timer_cb():
    global speedNum
    global is_increse
    if speedNum >= 110:
        is_increse = 0
    if speedNum <= 70:
        is_increse = 1
    speed_meter_board.set_indicator_value(speed_meter_board_scale_1_needleLine_0, speedNum)
    speed_label_digit.set_text(str(speedNum))
    if is_increse == 1:
        speedNum += 1
    else:
        speedNum -= 1

def screen_1_chart_timer_cb():
    ser_y_array = lv.chart.get_y_array(record_chart_board, chart_series_0)
    global spd_chart
    global is_up
    for i in range(99):
        spd_chart[i] = spd_chart[i+1]
        ser_y_array[i] = spd_chart[i]
    
    if spd_chart[99] > 110:
        is_up = 0
    if spd_chart[99] < 70:
        is_up = 1
    
    if is_up == 1:
        spd_chart[99] += lv.rand(0, 5)
    else:
        spd_chart[99] -= lv.rand(0, 5)
        
    ser_y_array[99] = spd_chart[99]
    lv.chart.refresh(record_chart_board)