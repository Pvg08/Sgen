#include "functiongraphicdrawer.h"
#include "ui_functiongraphicdrawer.h"

graphicThread* functionGraphicDrawer::mThread = 0;

functionGraphicDrawer::functionGraphicDrawer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::functionGraphicDrawer)
{
    ui->setupUi(this);
    block_change = false;

    #if defined(__ANDROID__)
    ui->ampSlider->setOrientation(Qt::Horizontal);
    ui->ampSlider->setVisible(false);
    ui->lcdNumber_koef->setVisible(false);
    ui->label_koef->setVisible(false);
    #endif

    widget_drawer = new MGraphicDrawSurface();
    widget_drawer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->verticalLayout->addWidget(widget_drawer);

    widget_fft_drawer = new MFftDrawSurface();
    widget_fft_drawer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->verticalLayout->addWidget(widget_fft_drawer);
    widget_fft_drawer->setVisible(false);
    widget_fft_drawer->setHidden(true);
    widget_fft_drawer->setTimerInterval(30);

    if (mThread==0) {
        mThread = new graphicThread();
        mThread->setInterval(30);
    }

    mThread->addGraphic(this);

    widget_drawer->setT(0.001);
    widget_drawer->setT0(0.001);
    widget_drawer->setFreq(500);
    widget_drawer->setAmp(1);
    widget_drawer->setKt(0.005);
    widget_drawer->resetGraphicFunctions();

    widget_fft_drawer->setT(0);
    widget_fft_drawer->setFreq(500);
    widget_fft_drawer->setAmp(1);
    widget_fft_drawer->setKt(0.005);
    widget_fft_drawer->resetGraphicFunctions();

    on_durationSlider_valueChanged(ui->durationSlider->value());
    on_ampSlider_valueChanged(ui->ampSlider->value());
}

functionGraphicDrawer::~functionGraphicDrawer()
{
    mThread->removeGraphic();
    if (mThread->getLinksCount()==0) {
        mThread->Stop = true;
        mThread->quit();
        delete mThread;
        mThread = 0;
    }
    delete widget_drawer;
    delete widget_fft_drawer;
    delete ui;
}

void functionGraphicDrawer::setGraphicFunction(GenSoundFunction value)
{
    widget_drawer->setGraphicFunction(value);
    widget_fft_drawer->setGraphicFunction(value);
}

void functionGraphicDrawer::setT0(double value)
{
    widget_drawer->setT0(value);
}

void functionGraphicDrawer::setAmp(double value)
{
    widget_drawer->setAmp(value);
    widget_fft_drawer->setAmp(value);
}

void functionGraphicDrawer::setFreq(double value)
{
    widget_drawer->setFreq(value);
    widget_fft_drawer->setFreq(value);
}

int functionGraphicDrawer::getDtIntValue() const
{
    return ui->durationSlider->value();
}

void functionGraphicDrawer::setDtIntValue(int value)
{
    block_change = true;
    ui->durationSlider->setValue(value);
    on_durationSlider_valueChanged(value);
    block_change = false;
}

int functionGraphicDrawer::getKampIntValue() const
{
    return ui->ampSlider->value();
}

void functionGraphicDrawer::setKampIntValue(int value)
{
    block_change = true;
    ui->ampSlider->setValue(value);
    on_ampSlider_valueChanged(value);
    block_change = false;
}

bool functionGraphicDrawer::isGrouped()
{
    return ui->checkBox_grouped->isChecked();
}

void functionGraphicDrawer::setGrouped(bool value)
{
    block_change = true;
    ui->checkBox_grouped->setChecked(value);
    block_change = false;
}

bool functionGraphicDrawer::isFft()
{
    return ui->checkBox_fft->isChecked();
}

void functionGraphicDrawer::setFft(bool value)
{
    block_change = true;
    ui->checkBox_fft->setChecked(value);
    block_change = false;
}

void functionGraphicDrawer::drawCycle()
{
    widget_drawer->incT();
    widget_drawer->update();
    ui->lcdNumber_t->display(widget_drawer->getT());
    widget_fft_drawer->incT();
    widget_fft_drawer->update();
}

void functionGraphicDrawer::run()
{
    drawCycle();
    mThread->Stop = false;
    mThread->start();
}

void functionGraphicDrawer::stop()
{
    mThread->Stop = true;
}

void functionGraphicDrawer::on_durationSlider_valueChanged(int value)
{
    widget_drawer->setDt(0.1 * value / ui->durationSlider->maximum());
    widget_fft_drawer->setDt(0.2 + 0.01 * floor(200 * value / ui->durationSlider->maximum()));
    ui->lcdNumber_dur->display((widget_drawer->isVisible() ? widget_drawer : widget_fft_drawer)->getDt());
    if (!block_change) {
        emit changed();
    }
}

void functionGraphicDrawer::on_ampSlider_valueChanged(int value)
{
    widget_drawer->setKamp(1.0 + (double) 2*value / ui->ampSlider->maximum());
    ui->lcdNumber_koef->display(widget_drawer->getKamp());
    if (!block_change) {
        emit changed();
    }
}

void functionGraphicDrawer::on_checkBox_grouped_stateChanged(int arg1)
{
    if (!block_change) {
        emit changed();
    }
}

void functionGraphicDrawer::on_checkBox_fft_stateChanged(int arg1)
{
    bool fft_mode = ui->checkBox_fft->isChecked();
    ui->widget_koef->setVisible(!fft_mode);
    ui->widget_t->setVisible(!fft_mode);
    ui->ampSlider->setVisible(!fft_mode);
    widget_drawer->setVisible(!fft_mode);
    widget_drawer->setHidden(fft_mode);
    widget_fft_drawer->setVisible(fft_mode);
    widget_fft_drawer->setHidden(!fft_mode);
    ui->lcdNumber_dur->display((widget_drawer->isVisible() ? widget_drawer : widget_fft_drawer)->getDt());
    if (!block_change) {
        emit changed();
    }
}
