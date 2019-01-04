#include "widget.h"
#include "ui_widget.h"

#include "Log/lclog.h"
#include <QDebug>

#include <QPainter>
#include <qrencode.h>
#include "zbar.h"

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#pragma region Includes and Imports
#include <windows.h>

#include <metahost.h>
#pragma comment(lib, "mscoree.lib")

// Import mscorlib.tlb (Microsoft Common Language Runtime Class Library).
#import "mscorlib.tlb" raw_interfaces_only                \
    high_property_prefixes("_get","_put","_putref")        \
    rename("ReportEvent", "InteropServices_ReportEvent")
using namespace mscorlib;
#pragma endregion

HRESULT hr;
ICLRMetaHost *pMetaHost = NULL;
ICLRRuntimeInfo *pRuntimeInfo = NULL;
ICorRuntimeHost *pCorRuntimeHost = NULL;
IUnknownPtr spAppDomainThunk = NULL;
_AppDomainPtr spDefaultAppDomain = NULL;

bstr_t bstrAssemblyName(L"WrapMChat");
_AssemblyPtr spAssembly = NULL;
bstr_t bstrClassName(L"WrapMChat.WrapMChat");
_TypePtr spType = NULL;
variant_t vtObject;

_MethodInfoPtr mInfo_GetQRCode;
bstr_t bstrMethodName_GetQRCode(L"GetQRCode");
SAFEARRAY *psaMethodArgs_GetQRCode = NULL;
variant_t vtStringRet_GetQRCode;

_MethodInfoPtr mInfo_SetBillInfo;
bstr_t bstrMethodName_SetBillInfo(L"SetBillInfo");
SAFEARRAY *psaMethodArgs_SetBillInfo = NULL;
variant_t vtStringRet_SetBillInfo;

_MethodInfoPtr mInfo_GenerateQRCode;
bstr_t bstrMethodName_GenerateQRCode(L"GenerateQRCode");
SAFEARRAY *psaMethodArgs_GenerateQRCode = NULL;
variant_t vtStringRet_GenerateQRCode;

_MethodInfoPtr mInfo_CheckQRCode;
bstr_t bstrMethodName_CheckQRCode(L"CheckQRCode");
SAFEARRAY *psaMethodArgs_CheckQRCode = NULL;
variant_t vtStringRet_CheckQRCode;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->label->clear();

    LOGIN->LogInfo("MChat","Load and start the .NET runtime v4.0.30319");
    hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
    if (FAILED(hr)) {
        LOGIN->LogInfo("MChat","CLRCreateInstance failed hr:"+QString::number(hr,16));
    }
    else {
        hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&pRuntimeInfo));
        if (FAILED(hr)) {
            LOGIN->LogInfo("MChat","ICLRMetaHost::GetRuntime failed hr:"+QString::number(hr,16));
        }
        else {
            BOOL fLoadable;
            hr = pRuntimeInfo->IsLoadable(&fLoadable);
            if (FAILED(hr)) {
                LOGIN->LogInfo("MChat","ICLRRuntimeInfo::IsLoadable failed hr:"+QString::number(hr,16));
            }
            else {
                if (!fLoadable) {
                    LOGIN->LogInfo("MChat",".NET runtime v4.0.30319 cannot be loaded");
                }
                else {
                    hr = pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost, IID_PPV_ARGS(&pCorRuntimeHost));
                    if (FAILED(hr)) {
                        LOGIN->LogInfo("MChat","ICLRRuntimeInfo::GetInterface failed hr:"+QString::number(hr,16));
                    }
                    else {
                        hr = pCorRuntimeHost->Start();
                        if (FAILED(hr)) {
                            LOGIN->LogInfo("MChat","CLR failed to start hr:"+QString::number(hr,16));
                        }
                        else {
                            hr = pCorRuntimeHost->GetDefaultDomain(&spAppDomainThunk);
                            if (FAILED(hr)) {
                                LOGIN->LogInfo("MChat","ICorRuntimeHost::GetDefaultDomain failed hr:"+QString::number(hr,16));
                            }
                            else {
                                hr = spAppDomainThunk->QueryInterface(IID_PPV_ARGS(&spDefaultAppDomain));
                                if (FAILED(hr)) {
                                    LOGIN->LogInfo("MChat","Failed to get default AppDomain hr:"+QString::number(hr,16));
                                }
                                else {
                                    hr = spDefaultAppDomain->Load_2(bstrAssemblyName, &spAssembly);
                                    if (FAILED(hr)) {
                                        LOGIN->LogInfo("MChat","Failed to load the assembly hr:"+QString::number(hr,16));
                                    }
                                    else {
                                        hr = spAssembly->GetType_2(bstrClassName, &spType);
                                        if (FAILED(hr)) {
                                            LOGIN->LogInfo("MChat","Failed to get the Type interface hr:"+QString::number(hr,16));
                                        }
                                        else {
                                            hr = spAssembly->CreateInstance(bstrClassName, &vtObject);
                                            if (FAILED(hr)) {
                                                LOGIN->LogInfo("MChat","Assembly::CreateInstance failed hr:"+QString::number(hr,16));
                                            }
                                            else {
                                                hr = spType->GetMethod_2(bstrMethodName_GetQRCode,static_cast<BindingFlags>(
                                                                             BindingFlags_InvokeMethod | BindingFlags_Instance | BindingFlags_Public),&mInfo_GetQRCode);
                                                if (FAILED(hr)) {
                                                    LOGIN->LogInfo("MChat GetQRCode","Failed to GetMethod_2 GetQRCode hr:"+QString::number(hr,16));
                                                }
                                                else {
                                                    psaMethodArgs_GetQRCode = SafeArrayCreateVector(VT_VARIANT, 0, 0);
                                                }

                                                hr = spType->GetMethod_2(bstrMethodName_SetBillInfo,static_cast<BindingFlags>(
                                                                             BindingFlags_InvokeMethod | BindingFlags_Instance | BindingFlags_Public),&mInfo_SetBillInfo);
                                                if (FAILED(hr)) {
                                                    LOGIN->LogInfo("MChat SetBillInfo","Failed to GetMethod_2 SetBillInfo hr:"+QString::number(hr,16));
                                                }
                                                else {
                                                    psaMethodArgs_SetBillInfo = SafeArrayCreateVector(VT_VARIANT, 0, 2);
                                                }

                                                hr = spType->GetMethod_2(bstrMethodName_GenerateQRCode,static_cast<BindingFlags>(
                                                                             BindingFlags_InvokeMethod | BindingFlags_Instance | BindingFlags_Public),&mInfo_GenerateQRCode);
                                                if (FAILED(hr)) {
                                                    LOGIN->LogInfo("MChat GenerateQRCode","Failed to GetMethod_2 GenerateQRCode hr:"+QString::number(hr,16));
                                                }
                                                else {
                                                    psaMethodArgs_GenerateQRCode = SafeArrayCreateVector(VT_VARIANT, 0, 0);
                                                }

                                                hr = spType->GetMethod_2(bstrMethodName_CheckQRCode,static_cast<BindingFlags>(
                                                                             BindingFlags_InvokeMethod | BindingFlags_Instance | BindingFlags_Public),&mInfo_CheckQRCode);
                                                if (FAILED(hr)) {
                                                    LOGIN->LogInfo("MChat CheckQRCode","Failed to GetMethod_2 CheckQRCode hr:"+QString::number(hr,16));
                                                }
                                                else {
                                                    psaMethodArgs_CheckQRCode = SafeArrayCreateVector(VT_VARIANT, 0, 0);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

Widget::~Widget()
{
    delete ui;

    LOGIN->freeData();

    if (pMetaHost)
    {
        pMetaHost->Release();
        pMetaHost = NULL;
    }
    if (pRuntimeInfo)
    {
        pRuntimeInfo->Release();
        pRuntimeInfo = NULL;
    }
    if (pCorRuntimeHost)
    {
        pCorRuntimeHost->Release();
        pCorRuntimeHost = NULL;
    }

    if (psaMethodArgs_GetQRCode) {
        SafeArrayDestroy(psaMethodArgs_GetQRCode);
        psaMethodArgs_GetQRCode = NULL;
    }

    if (psaMethodArgs_SetBillInfo) {
        SafeArrayDestroy(psaMethodArgs_SetBillInfo);
        psaMethodArgs_SetBillInfo = NULL;
    }

    if (psaMethodArgs_GenerateQRCode) {
        SafeArrayDestroy(psaMethodArgs_GenerateQRCode);
        psaMethodArgs_GenerateQRCode = NULL;
    }

    if (psaMethodArgs_CheckQRCode) {
        SafeArrayDestroy(psaMethodArgs_CheckQRCode);
        psaMethodArgs_CheckQRCode = NULL;
    }
}

//得到QRCode 并显示QRCode图片
void Widget::on_pushButton_clicked()
{
    VariantClear(&vtStringRet_GetQRCode);

    hr = mInfo_GetQRCode->Invoke_3(vtObject, psaMethodArgs_GetQRCode, &vtStringRet_GetQRCode);
    if (FAILED(hr)) {
        LOGIN->LogInfo("MChat GetQRCode","Failed to invoke GetQRCode hr:"+QString::number(hr,16));
    }

    QString str = QString::fromWCharArray(static_cast<PCWSTR>(vtStringRet_GetQRCode.bstrVal));

    qDebug()<<"MChat GetQRCode:"<<str;

    if(!str.isEmpty())
    {
        setQRData(str.toUtf8());

        ui->label->setPixmap(QPixmap::fromImage(qrImage));
    }
    else
    {
        ui->label->clear();
    }
}

//设置支付信息
void Widget::on_pushButton_2_clicked()
{
    double amount = 6000.0;
    QString info = "Coca Cola;1;2000|Milk;1;4000";

    std::string str = info.toStdString();
    std::wstring wstr(str.length(), L' ');
    std::copy(str.begin(), str.end(), wstr.begin());
    LPCWSTR cwInfo = wstr.c_str();

    variant_t vtDouble(amount);
    variant_t vtString(cwInfo);

    bool isArg = true;

    for(int i=0;i<2;i++)
    {
        LONG index = i;

        if(i == 0)
        {
            hr = SafeArrayPutElement(psaMethodArgs_SetBillInfo, &index, &vtDouble);
            if (FAILED(hr)) {
                isArg = false;
                LOGIN->LogInfo("MChat SetBillInfo","SafeArrayPutElement vtDoubleArg failed hr:"+QString::number(hr,16));
            }
        }
        else if(i == 1)
        {
            hr = SafeArrayPutElement(psaMethodArgs_SetBillInfo, &index, &vtString);
            if (FAILED(hr)) {
                isArg = false;
                LOGIN->LogInfo("MChat SetBillInfo","SafeArrayPutElement vtString failed hr:"+QString::number(hr,16));
            }
        }
    }

    if(isArg)
    {
        VariantClear(&vtStringRet_SetBillInfo);

        hr = mInfo_SetBillInfo->Invoke_3(vtObject, psaMethodArgs_SetBillInfo, &vtStringRet_SetBillInfo);
        if (FAILED(hr)) {
            LOGIN->LogInfo("MChat SetBillInfo","Failed to invoke SetBillInfo hr:"+QString::number(hr,16));
        }

        short boolval = vtStringRet_SetBillInfo.boolVal;

        bool ret = false;
        if(boolval == -1)
        {
            ret = true;
        }
        else if(boolval == 0)
        {
            ret = false;
        }

        qDebug()<<"MChat SetBillInfo:"<<ret;
    }
}

//生成QRCode
void Widget::on_pushButton_3_clicked()
{
    VariantClear(&vtStringRet_GenerateQRCode);

    hr = mInfo_GenerateQRCode->Invoke_3(vtObject, psaMethodArgs_GenerateQRCode, &vtStringRet_GenerateQRCode);
    if (FAILED(hr)) {
        LOGIN->LogInfo("MChat GenerateQRCode","Failed to invoke GenerateQRCode hr:"+QString::number(hr,16));
    }

    short boolval = vtStringRet_GenerateQRCode.boolVal;

    bool ret = false;
    if(boolval == -1)
    {
        ret = true;
    }
    else if(boolval == 0)
    {
        ret = false;
    }

    qDebug()<<"MChat GenerateQRCode:"<<ret;
}

//检查支付状态
void Widget::on_pushButton_4_clicked()
{
    VariantClear(&vtStringRet_CheckQRCode);

    hr = mInfo_CheckQRCode->Invoke_3(vtObject, psaMethodArgs_CheckQRCode, &vtStringRet_CheckQRCode);
    if (FAILED(hr)) {
        LOGIN->LogInfo("MChat CheckQRCode","Failed to invoke CheckQRCode hr:"+QString::number(hr,16));
    }

    short boolval = vtStringRet_CheckQRCode.boolVal;

    bool ret = false;
    if(boolval == -1)
    {
        ret = true;
    }
    else if(boolval == 0)
    {
        ret = false;
    }

    qDebug()<<"MChat CheckQRCode:"<<ret;
}

void Widget::setQRData(const QByteArray &data)
{
    qrImage = QImage(512, 512, QImage::Format_Mono);
    QPainter painter(&qrImage);
    QRcode *qrcode = QRcode_encodeString(data.constData(), 1, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (qrcode) {
        QColor fg(Qt::black);
        QColor bg(Qt::white);
        painter.setBrush(bg);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, 512, 512);
        painter.setBrush(fg);
        const int s = qrcode->width > 0 ? qrcode->width : 1;
        const qreal scale = 512.0 / s;
        for(int y = 0; y < s; y++){
            for(int x = 0; x < s; x++){
                if(qrcode->data[y * s + x] & 0x01){
                    const qreal rx1 = x * scale, ry1 = y * scale;
                    QRectF r(rx1, ry1, scale, scale);
                    painter.drawRects(&r,1);
                }
            }
        }
        QRcode_free(qrcode);
    }
    else {
        qWarning() << tr("Generating QR code failed.");
    }
}
