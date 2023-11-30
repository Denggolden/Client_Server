#pragma execution_character_set("UTF-8")
#ifndef STYLEDATA_H
#define STYLEDATA_H

#include <QObject>
#include <QBrush>

static QString MainWinToolBtnQss="QToolButton:hover{background-color: rgba(31, 59, 75,50);}\
        QToolButton::selected{background-color:rgba(31, 59, 75,50);}\
        QToolButton{color:white;background-color:rgba(0,0,0,0);}";

        static QString SampleToolBtnQss="QToolButton{background-color:rgba(0,0,0,0);}\
        QToolButton::selected{background-color:rgba(31, 59, 75,50);}\
        QToolButton{color:black;}";

        static QString ReagentInfoWinToolBtnQss="QToolButton{color:DeepSkyBlue;}";

static QString NormalBtnQss="QPushButton{background-color:DeepSkyBlue;\
        color: white;border-radius: 5px; border-style: outset;}"
"QPushButton:hover{background-color:white; color: black;}"
"QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}"
"QPushButton:disabled{background-color: lightgray; color: gray;}";

static QString LightBtnQss="QPushButton{background-color:PeachPuff;\
        color: black;border-radius: 3px;}";

static QString DeepBtnQss="QPushButton{background-color:DeepSkyBlue;\
        color: black;border-radius: 3px;}";

static QString LightBoardBtnQss="QPushButton{background-color:PeachPuff;\
        color: black;border-radius: 3px; border-style: outset; border: 2px groove red;}";

static QString DeepBoardBtnQss="QPushButton{background-color:DeepSkyBlue;\
        color: black;border-radius: 3px; border-style: outset; border: 2px groove red;}";

static QString ImgBtnQss="QPushButton{background-color:DeepSkyBlue;\
        color: white; border-radius: 5px;border-style: outset;qproperty-icon: url(:/Image/Import.png);}"
"QPushButton:hover{background-color:white; color: black;}"
"QPushButton:pressed{background-color:rgb(85, 170, 255);\
border-style: inset;}";

static QString ImgBtnQss1="QPushButton{background-color:white;color: Blue;\
        qproperty-icon: url(:/Image/SC1-4.png);border:3px solid rgb(85, 170, 255);border-radius: 3px;text-align:left;}"
"QPushButton:hover{background-color:DeepSkyBlue; color: black;}"
"QPushButton:pressed{background-color:white;\
border-style: inset;}";
static QString NormalBoardBtnQss="QPushButton{background-color:DeepSkyBlue;\
        color: white;   border-radius: 5px;  border: 2px groove red;border-style: outset;"
"QPushButton:hover{background-color:white; color: black;}"
"QPushButton:pressed{background-color:rgb(85, 170, 255);\
border-style: inset;}";

static QString TableWidgetQss = "QHeaderView::section{"
                                "border:1px solid #E0DDDC;"
                                "background-color:#33a3dc;"
                                "height:50px;"
                                "color:white;"
                                "}"
                                "QTableView , QTableWidget{"
                                "selection-background-color:#87CEFA;"
                                "background-color:white;"
                                "border:0px solid skyblue;"
                                "gridline-color:lightgray;"
                                "}";


static QString VerticalQss =
        "QScrollBar:vertical"
        "{"
        "    margin:20px 0px 20px 0px;"
        "    background-color:#ffffff;"
        "    border: 0px solid lightgray;"
        "    width:40px;"
        "    padding-left:0px;"
        "    padding-right:0px;"
        "}"
        "QScrollBar::handle:vertical"
        "{"
        "    background-color:#B0C4DE;"
        "    width:40px;"
        "    border-radius:3px;"
        "}"
        "QScrollBar::handle:vertical:hover"
        "{"
        "    background-color:#B0C4DE;"
        "    width:40px;"
        "    border-radius:3px;"
        "}"
        "QScrollBar::sub-line:vertical,QScrollBar::add-line:vertical"
        "{"
        "    subcontrol-origin: margin;"
        "    border: 0px solid lightgray;"
        "    height:20px;"
        "}"
        "QScrollBar::sub-page:vertical,QScrollBar::add-page:vertical"
        "{"
        "    border: 0px solid lightgray;"
        "    background-color:#ffffff;"
        "}"
        ;

//CheckBox
static QString CheckBoxQss ="QCheckBox{color:white}"
                            "QCheckBox::indicator {width: 30px;height: 30px;}"
                            "QCheckBox::indicator:checked {image: url(:/Image/checked1.png);}"
                            "QCheckBox::indicator:unchecked {image: url(:/Image/Unchecked1.png);}"
                            "QCheckBox::indicator:unchecked:pressed {image: url(:/Image/checked1.png);}";

static QString CheckBoxQssText ="QCheckBox{color:black}"
                            "QCheckBox::indicator {width: 30px;height: 30px;}"
                            "QCheckBox::indicator:checked {image: url(:/Image/checked1.png);}"
                            "QCheckBox::indicator:unchecked {image: url(:/Image/Unchecked1.png);}"
                            "QCheckBox::indicator:unchecked:pressed {image: url(:/Image/checked1.png);}";

static QString CheckBoxHeaderQss ="QCheckBox::indicator {width: 30px;height: 30px;}"
                                  "QCheckBox::indicator:checked {image: url(:/Image/checked1.png);}"
                                  "QCheckBox::indicator:unchecked {image: url(:/Image/Unchecked1.png);}"
                                  "QCheckBox::indicator:unchecked:pressed {image: url(:/Image/checked1.png);}"
                                  "QCheckBox{background-color:DodgerBlue;}";

static QString RadioButtonQss="QRadioButton::indicator{width: 30px;height: 30px;}"
                              "QRadioButton::indicator:unchecked {image: url(:/Image/RBtnUnChecked.png);}"
                              "QRadioButton::indicator:checked {image: url(:/Image/RBtnChecked.png);}"
        "QRadioButton{font:bold; font-size: 26px;color: rgb(0, 0, 0);}"
        "QRadioButton::checked{color:rgb(30,144,255);}"
        "QRadioButton::unchecked{color:rgb(0, 0, 0);}";

static QString StyleColor="F5DEB3";;
static QString  DateTimeEditQss=QString("QDateTimeEdit{border-radius: 6px;border: 3px solid #%1;padding:3px 0px 3px 6px;color: #000000;selection-background-color:#%1;}").arg(StyleColor);
/*                                        "QDateTimeEdit::drop-down{border-image: url(:/Image/arrowDown.png);width:12px;height:6px;}"
                                        "QDateTimeEdit::down-arrow{border-image: url(:/Image/arrowDown.png);width:12px;height:6px;}"*/
static QString CalendarWidgetQss=QString("QCalendarWidget{background-color:#FFFFFF;border: 1px solid #%1;}"
                                         "QCalendarWidget QAbstractItemView:enabled{color:#000000;"
                                                                                   "background-color:#ffffff;"
                                                                                   "selection-color: white;"
                                                                                   "selection-background-color:#%1;}"
                                         "QCalendarWidget QSpinBox#qt_calendar_yearedit{background:#ffffff;height:34px;width:125px;selection-background-color:#%1;}"
                                         "QCalendarWidget QToolButton{background-color:#FFFFFF;height:34px;width:125px;color:#000000;}"
                                         "QCalendarWidget QToolButton:hover{border: 1px solid #%1;}"
                                         "QCalendarWidget QToolButton::menu-indicator#qt_calendar_monthbutton{subcontrol-position: right center;subcontrol-origin: padding;}"
                                         "QCalendarWidget QToolButton QMenu{background-color:#FFFFFF;width:125px;border:1px solid #%1;}"
                                         "QCalendarWidget QToolButton QMenu::item:selected{color:#FFFFFF;background:#%1;}").arg(StyleColor);

//Idle=0,Apply,UnderTesting,SampleAdd,Complete,NoComplete
static QString SampleIdleImg=":/Image/SFree.png";
static QString SampleApplyImg=":/Image/SApply.png";
static QString SampleUnderTestingImg=":/Image/SUnderTest.png";
static QString SampleSampleAddImg=":/Image/SAdded.png";
static QString SampleCompleteImg=":/Image/SComplete.png";
static QString SampleNoCompleteImg=":/Image/SNoComplete.png";

//Free=0,Full,Exhaust,Insufficient,Failure,CalibrationFault,ScanFailed,Expiration
static QBrush BrushFree=QColor(248,248,255);
static QBrush BrushFull=QColor(0,255,0);
static QBrush BrushExhaust=QColor(255,0,0);
static QBrush BrushInsufficient=QColor(255,255,0);
static QBrush BrushFailure=QColor(105,105,105);
static QBrush BrushCalibrationFault=QColor(255,69,0);
static QBrush BrushScanFailed=QColor(255,181,197);
static QBrush BrushExpiration=QColor(139,137,137);

//Idle=0,Apply,UnderTesting,SampleAdd,Complete,NoComplete
static QBrush BrushIdle=QColor(248,248,255);
static QBrush BrushApply=QColor(100,149,237);
static QBrush BrushUnderTesting=QColor(255,255,0);
static QBrush BrushSampleAdd=QColor(244,164,96);
static QBrush BrushComplete=QColor(99,184,255);
static QBrush BrushNoComplete=QColor(255,0,0);

//QScrollBar:vertical{margin:16px 0px 16px 0px;background-color:rgb(11,54,117);border:0px;width:14px;}
//QScrollBar::handle:vertical{background-color:rgba(59,103,168,190);border-radius:7px;width:13px;}
//QScrollBar::handle:vertical:hover{background-color:rgba(59,103,168,220);}
//QScrollBar::sub-line:vertical{subcontrol-position:top;subcontrol-origin:margin;background-color:rgb(11,54,117);border:1px solid rgb(11,54,117);height:16px;}
//QScrollBar::add-line:vertical{subcontrol-position:bottom;subcontrol-origin:margin;background-color:rgb(11,54,117);border:1px solid rgb(11,54,117);height:16px;}
//QScrollBar::up-arrow:vertical{border-image:url(:/commonImg/Resources/image/common/arrowUp.png);width:12px;height:6px;}
//QScrollBar::down-arrow:vertical{border-image:url(:/commonImg/Resources/image/common/arrowDown.png);width:12px;height:6px;}
//QScrollBar::sub-page:vertical,QScrollBar::add-page:vertical{background-color:rgb(11,54,117);}


//QScrollBar:horizontal{margin:0px 16px 0px 16px;background-color:rgb(11,54,117);border:none;height:14px;}
//QScrollBar::handle:horizontal{background-color:rgba(59,103,168,190);border-radius:7px;height:14px;}
//QScrollBar::handle:horizontal:hover{background-color:rgba(59,103,168,220);}
//QScrollBar::add-line:horizontal{subcontrol-position: right;subcontrol-origin:margin;background-color:rgb(11,54,117);border:1px solid rgb(11,54,117);height:12px;width:6px;}
//QScrollBar::sub-line:horizontal{subcontrol-position: left;subcontrol-origin:margin;background-color:rgb(11,54,117);border:1px solid rgb(11,54,117);height:12px;width:6px;}
//QScrollBar::left-arrow:horizontal{border-image:url(:/commonImg/Resources/image/common/arrowLeft.png);width:6px;height:12px;}
//QScrollBar::right-arrow:horizontal{border-image:url(:/commonImg/Resources/image/common/arrowRight.png);width:6px;height:12px;}
//QScrollBar::sub-page:horizontal,QScrollBar::add-page:horizontal{background-color:rgb(11,54,117);}

#endif // STYLEDATA_H
