#ifndef WEATHER_H
#define WEATHER_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QByteArray>
#include <QEventLoop>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QFile>


QT_BEGIN_NAMESPACE
namespace Ui {
class Weather;
}
QT_END_NAMESPACE

class Weather : public QMainWindow
{
    Q_OBJECT

public:
    Weather(QWidget *parent = nullptr);
    ~Weather();
    QString condition = "snow";
    QString name_city = "";


private slots:
    void on_pushButton_clicked();


private:
    bool internet = true;
    Ui::Weather *ui;
    QNetworkAccessManager Manager;
    QString ap_key = "Your api key"; // please use your api key for Openweathermap website
    QJsonArray get_let_len(QString name_city){
        QNetworkRequest req("http://api.openweathermap.org/geo/1.0/direct?q="+name_city+"&limit=5&appid="+this->ap_key);
        QNetworkReply *reply = Manager.get(req);

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        QJsonArray jsonArray;
        if(reply->error() == QNetworkReply::NoError){
            this->internet = true;
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            jsonArray = jsonDoc.array();
            return jsonArray;
        }
        else{
            QMessageBox::warning(this , "Internet Connection" , "Your are not connect to internet to get information from server(or maybe you should use VPN)!");
            this->internet = false;
        }
    }
    QJsonObject get_weather(double lat , double lon){
        QNetworkRequest req("https://api.openweathermap.org/data/2.5/weather?lat="+QString::number(lat)+"&lon="+QString::number(lon)+"&appid="+this->ap_key);
        QNetworkReply *reply = Manager.get(req);

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        QJsonObject jsonArray;
        if(reply->error() == QNetworkReply::NoError){
            this->internet = true;
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            jsonArray = jsonDoc.object();
            return jsonArray;
        }
        else{
            QMessageBox::warning(this , "Internet Connection" , "Your are not connect to internet to get information from server(or maybe you should use VPN)!");
            this->internet = false;
        }
    }
    void set_weather(double lat , double lon);
    void managment(){
        double lat = 0 , lon = 0;
        if(this->name_city != ""){
            QJsonArray jsonArray = this->get_let_len(this->name_city);
            for(QJsonValue value:jsonArray){
                lat = value["lat"].toDouble();
                lon = value["lon"].toDouble();
                break;
            }
            this->set_weather(lat , lon);
        }
    }
    void write_on_file(){
        QDir file_dir = QCoreApplication::applicationDirPath();
        QFile file = file_dir.filePath("st.txt");
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out << this->name_city;
            file.flush();
            file.close();
        }
    }
    void read_on_file(){
        QDir file_dir = QCoreApplication::applicationDirPath();
        QFile file = file_dir.filePath("st.txt");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            this->name_city = in.readAll();
        }
    }
};
#endif // WEATHER_H
