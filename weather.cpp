#include "weather.h"
#include "ui_weather.h"

Weather::Weather(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Weather)
{
    ui->setupUi(this);
    ui->background->setStyleSheet("background-color:#222;color:white;");
    this->read_on_file();
    this->setCentralWidget(ui->background);
    // load and get api response
    this->managment();
}

Weather::~Weather()
{
    delete ui;
}

void Weather::on_pushButton_clicked()
{
    this->name_city = ui->input_city->text();
    this->managment();
    this->write_on_file();
}

void Weather::set_weather(double lat , double lon){
    if(!this->internet){
        return;
    }
    QJsonObject weather = this->get_weather(lat , lon);
    ui->show_temp->setText(QString::number(int(weather["main"].toObject()["temp"].toDouble()-273.15))+"°C");
    QString City_inf = "";
    for(QJsonValue value:this->get_let_len(this->name_city)){
        City_inf = value["country"].toString()+"-"+value["name"].toString();
        break;
    }
    ui->show_loc->setText(City_inf);
    ui->max_temp->setText("Max Temp : " + QString::number(int(weather["main"].toObject()["temp_max"].toDouble()-273.15))+"°C");
    ui->min_temp->setText("Min Temp : " + QString::number(int(weather["main"].toObject()["temp_min"].toDouble()-273.15))+"°C");
    if(weather["weather"].toObject()["main"] == "Rain"){
        ui->show_icon->setPixmap(QPixmap(":/icon/image/rain.png"));
    }
    else if(weather["weather"].toObject()["main"] == "Clouds"){
        ui->show_icon->setPixmap(QPixmap(":/icon/image/cloud.png"));
    }
    else if(weather["weather"].toObject()["main"] == "Snow"){
        ui->show_icon->setPixmap(QPixmap(":/icon/image/snow.png"));
    }
    else{
        ui->show_icon->setPixmap(QPixmap(":/icon/image/sun.png"));
    }
}
