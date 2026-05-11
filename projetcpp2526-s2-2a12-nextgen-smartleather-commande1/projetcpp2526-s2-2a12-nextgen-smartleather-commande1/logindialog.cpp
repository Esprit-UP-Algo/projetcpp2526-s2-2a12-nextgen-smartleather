#include "logindialog.h"
#include "connection.h"
#include "smtp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QMediaCaptureSession>
#include <QStandardPaths>
#include <QTextStream>
#include <QInputDialog>
#include <QSettings>
#include <QRegularExpression>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <cmath>

// ═══════════════════════════════════════════════════════════════════════════
// AuthGuard
// ═══════════════════════════════════════════════════════════════════════════
const int AuthGuard::kDurations[] = {30, 60, 300};

AuthGuard &AuthGuard::instance()
{
    static AuthGuard ag;
    return ag;
}

AuthGuard::AuthGuard() { load(); }

int AuthGuard::lockDuration() const
{
    int idx = qMin(m_level, 2);
    return kDurations[idx];
}

bool AuthGuard::isLocked() const
{
    if (!m_locked) return false;
    return secondsRemaining() > 0;
}

int AuthGuard::secondsRemaining() const
{
    if (!m_locked || !m_lockTime.isValid()) return 0;
    int elapsed = (int)m_lockTime.secsTo(QDateTime::currentDateTime());
    int remaining = lockDuration() - elapsed;
    return qMax(0, remaining);
}

bool AuthGuard::recordFailure()
{
    if (isLocked()) return true;
    ++m_failed;
    if (m_failed >= kMaxAttempts) {
        m_locked   = true;
        m_lockTime = QDateTime::currentDateTime();
        m_level = qMin(m_level + 1, 2);
        save();
        return true;
    }
    save();
    return false;
}

void AuthGuard::recordSuccess()
{
    m_failed = 0;
    m_locked = false;
    m_level  = 0;
    m_lockTime = QDateTime();
    save();
}

void AuthGuard::load()
{
    QSettings s("SmartLeather", "AuthGuard");
    m_failed   = s.value("failed", 0).toInt();
    m_level    = s.value("level",  0).toInt();
    m_locked   = s.value("locked", false).toBool();
    QString dt = s.value("lockTime", "").toString();
    if (!dt.isEmpty())
        m_lockTime = QDateTime::fromString(dt, Qt::ISODate);
    if (m_locked && secondsRemaining() == 0) {
        m_locked = false;
        m_failed = 0;
        save();
    }
}

void AuthGuard::save()
{
    QSettings s("SmartLeather", "AuthGuard");
    s.setValue("failed",   m_failed);
    s.setValue("level",    m_level);
    s.setValue("locked",   m_locked);
    s.setValue("lockTime", m_lockTime.isValid()
                               ? m_lockTime.toString(Qt::ISODate)
                               : QString());
}

// ═══════════════════════════════════════════════════════════════════════════
// FaceEngine
// ═══════════════════════════════════════════════════════════════════════════
QString FaceEngine::enrollPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/face_admin.png";
}

bool FaceEngine::hasEnrolled()
{
    return QFile::exists(enrollPath());
}

bool FaceEngine::enrollAdmin(const QImage &frame)
{
    QDir().mkpath(QFileInfo(enrollPath()).absolutePath());
    QImage scaled = frame.scaled(kSize, kSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
                         .convertToFormat(QImage::Format_Grayscale8);
    scaled = normalize(scaled);
    return scaled.save(enrollPath(), "PNG");
}

QImage FaceEngine::normalize(const QImage &img)
{
    QImage out = img.convertToFormat(QImage::Format_Grayscale8);
    double sum = 0;
    for (int y = 0; y < out.height(); ++y) {
        const uchar *row = out.constScanLine(y);
        for (int x = 0; x < out.width(); ++x)
            sum += row[x];
    }
    double mean = sum / (out.width() * out.height());

    double var = 0;
    for (int y = 0; y < out.height(); ++y) {
        const uchar *row = out.constScanLine(y);
        for (int x = 0; x < out.width(); ++x) {
            double d = row[x] - mean;
            var += d * d;
        }
    }
    double stddev = std::sqrt(var / (out.width() * out.height()));
    if (stddev < 1.0) stddev = 1.0;

    for (int y = 0; y < out.height(); ++y) {
        uchar *row = out.scanLine(y);
        for (int x = 0; x < out.width(); ++x) {
            double norm = ((row[x] - mean) / stddev) * 64.0 + 128.0;
            row[x] = (uchar)qBound(0.0, norm, 255.0);
        }
    }
    return out;
}

double FaceEngine::compare(const QImage &a, const QImage &b)
{
    QImage ia = normalize(a.scaled(kSize, kSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
                           .convertToFormat(QImage::Format_Grayscale8));
    QImage ib = normalize(b.scaled(kSize, kSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
                           .convertToFormat(QImage::Format_Grayscale8));

    double sumSq = 0.0;
    for (int y = 0; y < kSize; ++y) {
        const uchar *ra = ia.constScanLine(y);
        const uchar *rb = ib.constScanLine(y);
        for (int x = 0; x < kSize; ++x) {
            double diff = (double)ra[x] - (double)rb[x];
            sumSq += diff * diff;
        }
    }
    double maxSq = 255.0 * 255.0 * kSize * kSize;
    double simEuclid = 1.0 - std::sqrt(sumSq / maxSq);

    double meanA = 0, meanB = 0;
    int N = kSize * kSize;
    for (int y = 0; y < kSize; ++y) {
        const uchar *ra = ia.constScanLine(y);
        const uchar *rb = ib.constScanLine(y);
        for (int x = 0; x < kSize; ++x) {
            meanA += ra[x]; meanB += rb[x];
        }
    }
    meanA /= N; meanB /= N;

    double num = 0, denA = 0, denB = 0;
    for (int y = 0; y < kSize; ++y) {
        const uchar *ra = ia.constScanLine(y);
        const uchar *rb = ib.constScanLine(y);
        for (int x = 0; x < kSize; ++x) {
            double da = ra[x] - meanA;
            double db = rb[x] - meanB;
            num  += da * db;
            denA += da * da;
            denB += db * db;
        }
    }
    double denom = std::sqrt(denA * denB);
    double simNCC = (denom > 0) ? (num / denom + 1.0) / 2.0 : 0.0;

    const int blockSize = kSize / 4;
    double simBlocks = 0.0;
    int nBlocks = 0;
    for (int by = 0; by < 4; ++by) {
        for (int bx = 0; bx < 4; ++bx) {
            double bSumSq = 0;
            for (int y = by*blockSize; y < (by+1)*blockSize; ++y) {
                const uchar *ra = ia.constScanLine(y);
                const uchar *rb = ib.constScanLine(y);
                for (int x = bx*blockSize; x < (bx+1)*blockSize; ++x) {
                    double d = (double)ra[x] - (double)rb[x];
                    bSumSq += d * d;
                }
            }
            double bMax = 255.0*255.0*blockSize*blockSize;
            simBlocks += 1.0 - std::sqrt(bSumSq / bMax);
            ++nBlocks;
        }
    }
    simBlocks /= nBlocks;

    double final_score = 0.25 * simEuclid + 0.50 * simNCC + 0.25 * simBlocks;
    qDebug() << "FaceEngine — Euclid:" << simEuclid
             << "NCC:" << simNCC
             << "Blocks:" << simBlocks
             << "Final:" << final_score;
    return final_score;
}

bool FaceEngine::verify(const QImage &frame)
{
    if (!hasEnrolled()) return false;
    QImage ref(enrollPath());
    if (ref.isNull()) return false;
    double sim = compare(frame, ref);
    qDebug() << "FaceEngine similarity:" << sim << "(threshold:" << kThreshold << ")";
    return sim >= kThreshold;
}

// ═══════════════════════════════════════════════════════════════════════════
// FaceResultWidget
// ═══════════════════════════════════════════════════════════════════════════
FaceResultWidget::FaceResultWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(80, 80);
    m_anim = new QPropertyAnimation(this, "angle", this);
    m_anim->setStartValue(0);
    m_anim->setEndValue(360);
    m_anim->setDuration(900);
    m_anim->setLoopCount(-1);
}

void FaceResultWidget::showScanning() { m_state=1; m_anim->start(); update(); }
void FaceResultWidget::showSuccess()  { m_state=2; m_anim->stop();  update(); }
void FaceResultWidget::showFail()     { m_state=3; m_anim->stop();  update(); }
void FaceResultWidget::reset()        { m_state=0; m_anim->stop(); m_angle=0; update(); }

void FaceResultWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    const QRectF r(4,4,72,72);

    if (m_state == 1) {
        p.setPen(QPen(QColor(198,142,101), 4));
        p.setBrush(Qt::NoBrush);
        p.drawArc(r, m_angle*16, 270*16);
    } else if (m_state == 2) {
        p.setPen(QPen(QColor(46,160,67), 4));
        p.setBrush(QColor(240,255,240));
        p.drawEllipse(r);
        QPainterPath check;
        check.moveTo(20,40); check.lineTo(34,54); check.lineTo(58,26);
        p.setPen(QPen(QColor(46,160,67), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.setBrush(Qt::NoBrush);
        p.drawPath(check);
    } else if (m_state == 3) {
        p.setPen(QPen(QColor(200,50,50), 4));
        p.setBrush(QColor(255,240,240));
        p.drawEllipse(r);
        p.setPen(QPen(QColor(200,50,50), 5, Qt::SolidLine, Qt::RoundCap));
        p.drawLine(22,22,58,58);
        p.drawLine(58,22,22,58);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// FaceScanPopup
// ═══════════════════════════════════════════════════════════════════════════
FaceScanPopup::FaceScanPopup(const QString &username, const QString &mode, QWidget *parent)
    : QDialog(parent), m_mode(mode), m_username(username)
{
    setWindowTitle(mode == "enroll" ? "Enregistrer le visage admin" : "Face ID");
    setFixedSize(420, 500);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    auto *root = new QWidget(this);
    root->setGeometry(0,0,420,500);
    root->setStyleSheet("QWidget{background:#ffffff;border-radius:16px;}"
                        "QLabel{border:none;background:transparent;}");

    auto *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(32); shadow->setOffset(0,4);
    shadow->setColor(QColor(0,0,0,90));
    root->setGraphicsEffect(shadow);

    auto *vl = new QVBoxLayout(root);
    vl->setContentsMargins(20,16,20,16);
    vl->setSpacing(10);

    auto *titleRow = new QHBoxLayout();
    auto *titleLbl = new QLabel(mode == "enroll" ? "Enregistrer visage" : "Face ID", root);
    titleLbl->setStyleSheet("font-size:17px;font-weight:bold;color:#2c2c2c;");
    auto *btnX = new QPushButton("x", root);
    btnX->setFixedSize(28,28);
    btnX->setStyleSheet(
        "QPushButton{background:#f0f0f0;border:none;border-radius:14px;color:#666;font-size:13px;}"
        "QPushButton:hover{background:#e0e0e0;}");
    titleRow->addWidget(titleLbl); titleRow->addStretch(); titleRow->addWidget(btnX);
    vl->addLayout(titleRow);

    QString sub = (mode == "enroll")
        ? "Regardez la camera puis cliquez 'Capturer'"
        : (FaceEngine::hasEnrolled()
               ? "Regardez la camera pour vous identifier"
               : "Aucun visage enregistre. Utilisez 'Enregistrer' d'abord.");
    auto *subLbl = new QLabel(sub, root);
    subLbl->setStyleSheet("font-size:12px;color:#8c7b70;");
    subLbl->setAlignment(Qt::AlignCenter);
    subLbl->setWordWrap(true);
    vl->addWidget(subLbl);

    m_videoWidget = new QVideoWidget(root);
    m_videoWidget->setFixedSize(380, 280);
    m_videoWidget->setStyleSheet("background:#1a1a1a;border-radius:10px;");
    vl->addWidget(m_videoWidget, 0, Qt::AlignCenter);

    auto *resRow = new QHBoxLayout();
    resRow->setAlignment(Qt::AlignCenter);
    resRow->setSpacing(12);
    m_result = new FaceResultWidget(root);
    m_statusLabel = new QLabel(
        mode == "enroll" ? "Pret a capturer" : "Pret a scanner", root);
    m_statusLabel->setStyleSheet("font-size:12px;color:#8c7b70;");
    m_statusLabel->setWordWrap(true);
    resRow->addWidget(m_result);
    resRow->addWidget(m_statusLabel, 1);
    vl->addLayout(resRow);

    auto *btnRow = new QHBoxLayout();
    QString btnLabel = (mode == "enroll") ? "  Capturer mon visage" : "  Scanner";
    m_btnScan = new QPushButton(btnLabel, root);
    m_btnScan->setFixedHeight(42);
    m_btnScan->setStyleSheet(
        "QPushButton{background:#7B3F00;color:white;border:none;border-radius:8px;"
        "font-size:13px;font-weight:bold;}"
        "QPushButton:hover{background:#8B4513;}"
        "QPushButton:disabled{background:#ccc;color:#888;}");
    auto *btnCancel = new QPushButton("Annuler", root);
    btnCancel->setFixedHeight(42);
    btnCancel->setStyleSheet(
        "QPushButton{background:#f0ebe6;color:#7B3F00;border:1px solid #C68E65;"
        "border-radius:8px;font-size:13px;}"
        "QPushButton:hover{background:#e5ddd6;}");
    btnRow->addWidget(m_btnScan); btnRow->addWidget(btnCancel);
    vl->addLayout(btnRow);

    if (mode == "verify" && !FaceEngine::hasEnrolled()) {
        m_btnScan->setEnabled(false);
        m_statusLabel->setText("Aucun visage enregistre. Utilisez 'Enregistrer' d'abord.");
        m_statusLabel->setStyleSheet("font-size:11px;color:#c0392b;");
    }

    m_scanTimer = new QTimer(this);
    m_scanTimer->setSingleShot(true);
    connect(m_scanTimer, &QTimer::timeout, this, &FaceScanPopup::captureAndProcess);

    connect(btnX,      &QPushButton::clicked, this, [=](){ stopCamera(); reject(); });
    connect(btnCancel, &QPushButton::clicked, this, [=](){ stopCamera(); reject(); });

    connect(m_btnScan, &QPushButton::clicked, this, [=](){
        if (m_scanning) return;
        m_scanning = true;
        m_btnScan->setEnabled(false);
        m_result->showScanning();
        m_statusLabel->setText(mode == "enroll"
            ? "Restez immobile... capture dans 3s"
            : "Analyse en cours... restez immobile");
        m_statusLabel->setStyleSheet("font-size:12px;color:#C68E65;font-weight:bold;");
        m_scanTimer->start(3000);
    });

    startCamera();
}

FaceScanPopup::~FaceScanPopup() { stopCamera(); }

void FaceScanPopup::startCamera()
{
    if (m_camera) return;
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (cameras.isEmpty()) {
        m_statusLabel->setText("Aucune camera detectee.");
        m_statusLabel->setStyleSheet("font-size:12px;color:#c0392b;");
        if (m_btnScan) m_btnScan->setEnabled(false);
        return;
    }
    m_camera  = new QCamera(cameras.first(), this);
    m_session = new QMediaCaptureSession(this);
    m_sink    = new QVideoSink(this);
    m_session->setCamera(m_camera);
    m_session->setVideoOutput(m_videoWidget);
    m_session->setVideoSink(m_sink);
    connect(m_sink, &QVideoSink::videoFrameChanged,
            this, &FaceScanPopup::onNewFrame);
    m_camera->start();
}

void FaceScanPopup::stopCamera()
{
    if (m_camera) { m_camera->stop(); delete m_camera; m_camera = nullptr; }
    if (m_session){ delete m_session; m_session = nullptr; }
    if (m_sink)   { delete m_sink;    m_sink    = nullptr; }
}

void FaceScanPopup::onNewFrame(const QVideoFrame &frame)
{
    QVideoFrame f = frame;
    QImage img = f.toImage();
    if (!img.isNull())
        m_lastFrame = img;
}

void FaceScanPopup::captureAndProcess()
{
    m_scanning = false;

    if (m_lastFrame.isNull()) {
        m_result->showFail();
        m_statusLabel->setText("Impossible de capturer l'image.");
        m_statusLabel->setStyleSheet("font-size:12px;color:#c0392b;");
        if (m_btnScan) m_btnScan->setEnabled(true);
        return;
    }

    if (m_mode == "enroll") {
        bool ok = FaceEngine::enrollAdmin(m_lastFrame);
        if (ok) {
            m_result->showSuccess();
            m_statusLabel->setText("Visage admin enregistre avec succes !");
            m_statusLabel->setStyleSheet("font-size:13px;color:#2ea043;font-weight:bold;");
            m_accepted = true;
            stopCamera();
            QTimer::singleShot(1500, this, &QDialog::accept);
        } else {
            m_result->showFail();
            m_statusLabel->setText("Erreur lors de l'enregistrement.");
            m_statusLabel->setStyleSheet("font-size:12px;color:#c0392b;");
            if (m_btnScan) m_btnScan->setEnabled(true);
        }
    } else {
        bool ok = FaceEngine::verify(m_lastFrame);
        if (ok) {
            m_result->showSuccess();
            m_statusLabel->setText("Visage reconnu — Bienvenue !");
            m_statusLabel->setStyleSheet("font-size:13px;color:#2ea043;font-weight:bold;");
            m_accepted = true;
            stopCamera();
            QTimer::singleShot(1200, this, &QDialog::accept);
        } else {
            m_result->showFail();
            m_statusLabel->setText("Visage non reconnu — Acces refuse.");
            m_statusLabel->setStyleSheet("font-size:12px;color:#c0392b;font-weight:bold;");
            if (m_btnScan) { m_btnScan->setEnabled(true); m_result->reset(); }
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// LoginDialog
// ═══════════════════════════════════════════════════════════════════════════
LoginDialog::LoginDialog(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Smart Leather Factory - Login");
    resize(1024, 680);

    QScreen *screen = QApplication::primaryScreen();
    if (screen) move(screen->availableGeometry().center() - rect().center());

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color:#f7f3ee;");
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setAlignment(Qt::AlignCenter);

    QFrame *card = new QFrame(central);
    card->setFixedWidth(860);
    card->setStyleSheet("QFrame{background:#fff;border:1px solid #e0d5cc;border-radius:12px;}");

    QVBoxLayout *cl = new QVBoxLayout(card);
    cl->setContentsMargins(60,40,60,40);
    cl->setSpacing(12);

    auto *title = new QLabel("Connexion", card);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:16px;font-weight:bold;color:#2c2c2c;border:none;background:transparent;");
    cl->addWidget(title);

    // Bandeau de blocage
    m_lockLabel = new QLabel("", card);
    m_lockLabel->setAlignment(Qt::AlignCenter);
    m_lockLabel->setFixedHeight(36);
    m_lockLabel->setStyleSheet(
        "background:#fff3cd;border:1px solid #ffc107;border-radius:6px;"
        "color:#856404;font-size:12px;font-weight:bold;padding:0 10px;");
    m_lockLabel->setVisible(false);
    cl->addWidget(m_lockLabel);

    m_userEdit = new QLineEdit(card);
    m_userEdit->setPlaceholderText("Adresse e-mail (ex: admin@smartleather.com)");
    m_userEdit->setFixedHeight(46);
    m_userEdit->setStyleSheet(
        "QLineEdit{border:1.5px solid #C68E65;border-radius:6px;padding:0 14px;"
        "font-size:13px;color:#2c2c2c;background:#fff;}"
        "QLineEdit:focus{border:2px solid #8B4513;}"
        "QLineEdit:disabled{background:#f5f5f5;color:#aaa;}");
    cl->addWidget(m_userEdit);

    m_passEdit = new QLineEdit(card);
    m_passEdit->setPlaceholderText("Mot de passe");
    m_passEdit->setEchoMode(QLineEdit::Password);
    m_passEdit->setFixedHeight(46);
    m_passEdit->setStyleSheet(
        "QLineEdit{border:1.5px solid #d0c8c0;border-radius:6px;padding:0 14px;"
        "font-size:13px;color:#2c2c2c;background:#fff;}"
        "QLineEdit:focus{border:2px solid #8B4513;}"
        "QLineEdit:disabled{background:#f5f5f5;color:#aaa;}");
    cl->addWidget(m_passEdit);

    m_errorLabel = new QLabel("", card);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setStyleSheet("color:#c0392b;font-size:12px;border:none;background:transparent;");
    m_errorLabel->setVisible(false);
    cl->addWidget(m_errorLabel);

    m_btnLogin = new QPushButton("Se connecter", card);
    m_btnLogin->setFixedHeight(46);
    m_btnLogin->setCursor(Qt::PointingHandCursor);
    m_btnLogin->setStyleSheet(
        "QPushButton{background:#7B3F00;color:white;border:none;border-radius:6px;"
        "font-size:14px;font-weight:bold;}"
        "QPushButton:hover{background:#8B4513;}"
        "QPushButton:disabled{background:#ccc;color:#888;}");
    cl->addWidget(m_btnLogin);

    // Separateur
    auto *sepRow = new QHBoxLayout();
    auto *sepL = new QFrame(card); sepL->setFrameShape(QFrame::HLine);
    sepL->setStyleSheet("color:#d0c8c0;");
    auto *sepR = new QFrame(card); sepR->setFrameShape(QFrame::HLine);
    sepR->setStyleSheet("color:#d0c8c0;");
    auto *sepLbl = new QLabel("ou", card);
    sepLbl->setStyleSheet("color:#8c7b70;font-size:12px;border:none;background:transparent;");
    sepLbl->setAlignment(Qt::AlignCenter);
    sepRow->addWidget(sepL); sepRow->addWidget(sepLbl); sepRow->addWidget(sepR);
    cl->addLayout(sepRow);

    auto *faceRow = new QHBoxLayout();
    faceRow->setSpacing(10);

    m_btnFace = new QPushButton("  Connexion Face ID", card);
    m_btnFace->setFixedHeight(46);
    m_btnFace->setCursor(Qt::PointingHandCursor);
    m_btnFace->setStyleSheet(
        "QPushButton{background:#f7f3ee;color:#7B3F00;border:1.5px solid #C68E65;"
        "border-radius:6px;font-size:13px;font-weight:600;}"
        "QPushButton:hover{background:#ede5dc;}"
        "QPushButton:disabled{background:#f0f0f0;color:#aaa;border-color:#ccc;}");

    auto *btnEnroll = new QPushButton("  Enregistrer visage", card);
    btnEnroll->setFixedHeight(46);
    btnEnroll->setCursor(Qt::PointingHandCursor);
    btnEnroll->setStyleSheet(
        "QPushButton{background:#f7f3ee;color:#7B3F00;border:1.5px solid #C68E65;"
        "border-radius:6px;font-size:13px;font-weight:600;}"
        "QPushButton:hover{background:#ede5dc;}");

    faceRow->addWidget(m_btnFace);
    faceRow->addWidget(btnEnroll);
    cl->addLayout(faceRow);

    auto *enrollStatus = new QLabel(
        FaceEngine::hasEnrolled() ? "Visage enregistre" : "", card);
    enrollStatus->setAlignment(Qt::AlignCenter);
    enrollStatus->setStyleSheet("font-size:11px;color:#2ea043;border:none;background:transparent;");
    cl->addWidget(enrollStatus);

    auto *btnCreate = new QPushButton("Creer un compte", card);
    btnCreate->setFlat(true);
    btnCreate->setCursor(Qt::PointingHandCursor);
    btnCreate->setStyleSheet(
        "QPushButton{color:#8B4513;font-size:13px;text-decoration:underline;"
        "border:none;background:transparent;}"
        "QPushButton:hover{color:#5a2d00;}");

    auto *btnForgot = new QPushButton("Mot de passe oublie ?", card);
    btnForgot->setFlat(true);
    btnForgot->setCursor(Qt::PointingHandCursor);
    btnForgot->setStyleSheet(
        "QPushButton{color:#8c7b70;font-size:12px;text-decoration:underline;"
        "border:none;background:transparent;}"
        "QPushButton:hover{color:#5a2d00;}");

    auto *bottomRow = new QHBoxLayout();
    bottomRow->addWidget(btnForgot, 0, Qt::AlignLeft);
    bottomRow->addWidget(btnCreate, 0, Qt::AlignRight);
    cl->addLayout(bottomRow);

    mainLayout->addWidget(card);

    // Timer compte a rebours
    m_lockTimer = new QTimer(this);
    m_lockTimer->setInterval(1000);
    connect(m_lockTimer, &QTimer::timeout, this, &LoginDialog::updateLockUI);
    m_lockTimer->start();

    updateLockUI();

    connect(m_btnLogin, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(btnCreate,  &QPushButton::clicked, this, &LoginDialog::onCreateAccount);
    connect(btnForgot,  &QPushButton::clicked, this, &LoginDialog::onForgotPassword);
    connect(m_passEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLogin);
    connect(m_userEdit, &QLineEdit::returnPressed, this, [=]{ m_passEdit->setFocus(); });

    // Face ID — connexion
    connect(m_btnFace, &QPushButton::clicked, this, [=](){
        if (AuthGuard::instance().isLocked()) return;
        const QString user = m_userEdit->text().trimmed();
        FaceScanPopup popup(user, "verify", this);
        if (popup.exec() == QDialog::Accepted && popup.accepted()) {
            AuthGuard::instance().recordSuccess();
            m_loggedEmail = m_userEdit->text().trimmed().toLower();
            m_accepted = true;
            close();
        } else {
            bool locked = AuthGuard::instance().recordFailure();
            if (locked) {
                updateLockUI();
            } else {
                int rem = AuthGuard::kMaxAttempts - AuthGuard::instance().failedAttempts();
                m_errorLabel->setText(QString("Visage non reconnu. %1 tentative(s) restante(s).").arg(rem));
                m_errorLabel->setVisible(true);
            }
        }
    });

    // Face ID — enregistrement
    connect(btnEnroll, &QPushButton::clicked, this, [=](){
        FaceScanPopup popup("", "enroll", this);
        if (popup.exec() == QDialog::Accepted && popup.accepted()) {
            enrollStatus->setText("Visage enregistre");
            QMessageBox::information(this, "Face ID", "Visage enregistre avec succes !");
        }
    });
}

void LoginDialog::onLogin()
{
    if (AuthGuard::instance().isLocked()) return;

    const QString user = m_userEdit->text().trimmed();
    const QString pass = m_passEdit->text();

    if (user.isEmpty() || pass.isEmpty()) {
        m_errorLabel->setText("Veuillez remplir tous les champs.");
        m_errorLabel->setVisible(true);
        return;
    }

    QRegularExpression emailRx(R"(^[A-Za-z0-9._%+\-]+@[A-Za-z0-9.\-]+\.[A-Za-z]{2,}$)");
    if (!emailRx.match(user).hasMatch()) {
        m_errorLabel->setText("Entrez une adresse e-mail valide.");
        m_errorLabel->setVisible(true);
        return;
    }

    if (validateCredentials(user, pass)) {
        AuthGuard::instance().recordSuccess();
        m_loggedEmail = user.toLower();
        m_accepted = true;
        close();
    } else {
        bool locked = AuthGuard::instance().recordFailure();
        if (locked) {
            updateLockUI();
        } else {
            int rem = AuthGuard::kMaxAttempts - AuthGuard::instance().failedAttempts();
            m_errorLabel->setText(
                QString("Identifiants incorrects. %1 tentative(s) restante(s).").arg(rem));
            m_errorLabel->setVisible(true);
            m_passEdit->clear();
            m_passEdit->setFocus();
        }
    }
}

void LoginDialog::updateLockUI()
{
    AuthGuard &ag = AuthGuard::instance();
    if (ag.isLocked()) {
        int secs = ag.secondsRemaining();
        int mins = secs / 60;
        QString msg = mins > 0
            ? QString("Acces bloque — reessayez dans %1 min %2 s").arg(mins).arg(secs % 60)
            : QString("Acces bloque — reessayez dans %1 secondes").arg(secs);
        m_lockLabel->setText(msg);
        m_lockLabel->setVisible(true);
        m_errorLabel->setVisible(false);
        setInputsEnabled(false);
    } else {
        m_lockLabel->setVisible(false);
        setInputsEnabled(true);
    }
}

void LoginDialog::setInputsEnabled(bool enabled)
{
    m_userEdit->setEnabled(enabled);
    m_passEdit->setEnabled(enabled);
    m_btnLogin->setEnabled(enabled);
    m_btnFace->setEnabled(enabled);
}

void LoginDialog::onCreateAccount()
{
    bool ok;
    QString email = QInputDialog::getText(
        this, "Creer un compte", "Email :", QLineEdit::Normal, "", &ok);
    if (!ok || email.isEmpty()) return;

    if (emailExists(email)) {
        QMessageBox::warning(this, "Erreur", "Cet email est deja enregistre.");
        return;
    }

    QString password = QInputDialog::getText(
        this, "Creer un compte", "Mot de passe :", QLineEdit::Password, "", &ok);
    if (!ok || password.isEmpty()) return;

    QString confirmPassword = QInputDialog::getText(
        this, "Creer un compte", "Confirmer le mot de passe :", QLineEdit::Password, "", &ok);
    if (!ok || confirmPassword != password) {
        QMessageBox::warning(this, "Erreur", "Les mots de passe ne correspondent pas.");
        return;
    }

    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        Connection::instance()->createConnect();
        db = Connection::instance()->database();
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO UTILISATEUR (EMAIL, PASSWORD) VALUES (:email, :password)");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de creer le compte: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succes", "Compte cree avec succes. Vous pouvez vous connecter.");
    m_userEdit->setText(email);
    m_passEdit->clear();
    m_userEdit->setFocus();
}

bool LoginDialog::validateCredentials(const QString &user, const QString &pass)
{
    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        Connection::instance()->createConnect();
        db = Connection::instance()->database();
    }

    if (!db.isValid() || !db.isOpen()) {
        m_errorLabel->setText("Erreur de connexion a la base de donnees.");
        m_errorLabel->setVisible(true);
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT PASSWORD FROM UTILISATEUR WHERE EMAIL = :email");
    query.bindValue(":email", user);

    if (!query.exec()) {
        m_errorLabel->setText("Erreur d'authentification.");
        m_errorLabel->setVisible(true);
        return false;
    }

    if (!query.next()) {
        return false;
    }

    const QString storedPassword = query.value(0).toString();
    return storedPassword == pass;
}

bool LoginDialog::emailExists(const QString &email) const
{
    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        Connection::instance()->createConnect();
        db = Connection::instance()->database();
    }

    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM UTILISATEUR WHERE EMAIL = :email");
    query.bindValue(":email", email);

    if (!query.exec()) return false;
    return query.next();
}

void LoginDialog::onForgotPassword()
{
    bool ok = false;
    QString email = QInputDialog::getText(this, "Mot de passe oublie",
        "Entrez votre adresse e-mail :", QLineEdit::Normal, "", &ok);
    if (!ok || email.trimmed().isEmpty()) return;
    email = email.trimmed().toLower();

    QRegularExpression emailRx(R"(^[A-Za-z0-9._%+\-]+@[A-Za-z0-9.\-]+\.[A-Za-z]{2,}$)");
    if (!emailRx.match(email).hasMatch()) {
        QMessageBox::warning(this, "Email invalide", "Format d'email incorrect.");
        return;
    }

    // Generer un code a 6 chiffres
    QString code;
    for (int i = 0; i < 6; ++i)
        code += QString::number(QRandomGenerator::global()->bounded(10));

    QDateTime expiry = QDateTime::currentDateTime().addSecs(300);

    // Charger les credentials SMTP depuis .env.local
    auto loadKey = [](const QString &key) -> QString {
        const QStringList paths = {
            QCoreApplication::applicationDirPath() + "/../../../.env.local",
            QCoreApplication::applicationDirPath() + "/../../.env.local",
            QCoreApplication::applicationDirPath() + "/../.env.local",
            QCoreApplication::applicationDirPath() + "/.env.local"
        };
        for (const QString &p : paths) {
            QFile f(p);
            if (!f.open(QIODevice::ReadOnly)) continue;
            QTextStream ts(&f);
            while (!ts.atEnd()) {
                QString line = ts.readLine().trimmed();
                if (line.startsWith('#') || line.isEmpty()) continue;
                if (line.startsWith(key + "="))
                    return line.mid(key.length() + 1).trimmed();
            }
        }
        return {};
    };

    QString smtpUser = loadKey("SMTP_USER");
    if (smtpUser.isEmpty()) smtpUser = loadKey("EMAIL_USER");
    QString smtpPass = loadKey("SMTP_PASS");
    if (smtpPass.isEmpty()) smtpPass = loadKey("EMAIL_PASS");

    QString body = QString(
        "Bonjour,\n\n"
        "Votre code de verification SmartLeather est :\n\n"
        "    %1\n\n"
        "Ce code expire dans 5 minutes.\n"
        "Si vous n'avez pas demande ce code, ignorez cet email.\n\n"
        "-- SmartLeather Security"
    ).arg(code);

    auto *smtp = new SmtpClient(this);
    connect(smtp, &SmtpClient::emailSent, smtp, &QObject::deleteLater);

    if (!smtpUser.isEmpty() && !smtpPass.isEmpty()) {
        smtp->sendEmail(smtpUser, email,
                        "SmartLeather - Code de verification", body,
                        "smtp.gmail.com", 587, smtpUser, smtpPass);
        QMessageBox::information(this, "Code envoye",
            QString("Un code de verification a ete envoye a %1.\nIl expire dans 5 minutes.").arg(email));
    } else {
        smtp->deleteLater();
        QMessageBox::information(this, "Mode developpement",
            QString("SMTP non configure.\n\nVotre code de verification est :\n\n%1\n\n"
                    "Pour activer l'envoi email, configurez SMTP_USER et SMTP_PASS dans .env.local")
                .arg(code));
    }

    // Popup de saisie du code
    auto *dlg = new QDialog(this);
    dlg->setWindowTitle("Verification");
    dlg->setFixedSize(380, 280);
    dlg->setStyleSheet("QDialog{background:#f7f3ee;} QLabel{color:#3b2a20;}");

    auto *vl = new QVBoxLayout(dlg);
    vl->setContentsMargins(30,24,30,24);
    vl->setSpacing(12);

    auto *lbl = new QLabel("Entrez le code recu par email :", dlg);
    lbl->setStyleSheet("font-size:13px;font-weight:bold;");
    vl->addWidget(lbl);

    auto *codeEdit = new QLineEdit(dlg);
    codeEdit->setPlaceholderText("Code a 6 chiffres");
    codeEdit->setMaxLength(6);
    codeEdit->setAlignment(Qt::AlignCenter);
    codeEdit->setFixedHeight(46);
    codeEdit->setStyleSheet(
        "QLineEdit{border:1.5px solid #C68E65;border-radius:6px;"
        "font-size:20px;font-weight:bold;letter-spacing:6px;color:#3b2a20;background:#fff;}"
        "QLineEdit:focus{border:2px solid #8B4513;}");
    vl->addWidget(codeEdit);

    auto *errLbl = new QLabel("", dlg);
    errLbl->setStyleSheet("color:#c0392b;font-size:11px;");
    errLbl->setAlignment(Qt::AlignCenter);
    vl->addWidget(errLbl);

    auto *timerLbl = new QLabel("Expire dans 5:00", dlg);
    timerLbl->setStyleSheet("color:#8c7b70;font-size:11px;");
    timerLbl->setAlignment(Qt::AlignCenter);
    vl->addWidget(timerLbl);

    auto *btnVerify = new QPushButton("Verifier", dlg);
    btnVerify->setFixedHeight(42);
    btnVerify->setStyleSheet(
        "QPushButton{background:#7B3F00;color:white;border:none;border-radius:6px;"
        "font-size:13px;font-weight:bold;}"
        "QPushButton:hover{background:#8B4513;}");
    vl->addWidget(btnVerify);

    int *attempts = new int(0);
    auto *countdown = new QTimer(dlg);
    countdown->setInterval(1000);
    QDateTime *exp = new QDateTime(expiry);
    connect(countdown, &QTimer::timeout, dlg, [=](){
        int secs = (int)QDateTime::currentDateTime().secsTo(*exp);
        if (secs <= 0) {
            timerLbl->setText("Code expire");
            timerLbl->setStyleSheet("color:#c0392b;font-size:11px;font-weight:bold;");
            btnVerify->setEnabled(false);
            countdown->stop();
        } else {
            timerLbl->setText(QString("Expire dans %1:%2")
                .arg(secs/60).arg(secs%60, 2, 10, QChar('0')));
        }
    });
    countdown->start();

    connect(btnVerify, &QPushButton::clicked, dlg, [=](){
        if (QDateTime::currentDateTime() > *exp) {
            errLbl->setText("Code expire. Recommencez.");
            return;
        }
        if (*attempts >= 3) {
            errLbl->setText("Trop de tentatives. Recommencez depuis le debut.");
            btnVerify->setEnabled(false);
            return;
        }
        if (codeEdit->text().trimmed() != code) {
            ++(*attempts);
            int rem = 3 - *attempts;
            errLbl->setText(rem > 0
                ? QString("Code incorrect. %1 tentative(s) restante(s).").arg(rem)
                : "Trop de tentatives. Recommencez.");
            if (*attempts >= 3) btnVerify->setEnabled(false);
            return;
        }

        countdown->stop();
        dlg->accept();

        // Popup nouveau mot de passe
        auto *pwdDlg = new QDialog(this);
        pwdDlg->setWindowTitle("Nouveau mot de passe");
        pwdDlg->setFixedSize(360, 220);
        pwdDlg->setStyleSheet("QDialog{background:#f7f3ee;} QLabel{color:#3b2a20;}");

        auto *pvl = new QVBoxLayout(pwdDlg);
        pvl->setContentsMargins(28,20,28,20);
        pvl->setSpacing(10);

        pvl->addWidget(new QLabel("Nouveau mot de passe :", pwdDlg));
        auto *p1 = new QLineEdit(pwdDlg);
        p1->setEchoMode(QLineEdit::Password);
        p1->setFixedHeight(42);
        p1->setStyleSheet("QLineEdit{border:1.5px solid #C68E65;border-radius:6px;"
                          "padding:0 12px;font-size:13px;background:#fff;}");
        pvl->addWidget(p1);

        pvl->addWidget(new QLabel("Confirmer :", pwdDlg));
        auto *p2 = new QLineEdit(pwdDlg);
        p2->setEchoMode(QLineEdit::Password);
        p2->setFixedHeight(42);
        p2->setStyleSheet(p1->styleSheet());
        pvl->addWidget(p2);

        auto *pwdErr = new QLabel("", pwdDlg);
        pwdErr->setStyleSheet("color:#c0392b;font-size:11px;");
        pvl->addWidget(pwdErr);

        auto *btnSave = new QPushButton("Enregistrer", pwdDlg);
        btnSave->setFixedHeight(40);
        btnSave->setStyleSheet(
            "QPushButton{background:#7B3F00;color:white;border:none;border-radius:6px;"
            "font-size:13px;font-weight:bold;}"
            "QPushButton:hover{background:#8B4513;}");
        pvl->addWidget(btnSave);

        connect(btnSave, &QPushButton::clicked, pwdDlg, [=](){
            if (p1->text().length() < 6) {
                pwdErr->setText("Minimum 6 caracteres.");
                return;
            }
            if (p1->text() != p2->text()) {
                pwdErr->setText("Les mots de passe ne correspondent pas.");
                return;
            }
            // Mettre a jour en base de donnees
            QSqlDatabase db = Connection::instance()->database();
            if (!db.isOpen()) Connection::instance()->createConnect();
            QSqlQuery q(db);
            q.prepare("UPDATE UTILISATEUR SET PASSWORD = :pwd WHERE EMAIL = :email");
            q.bindValue(":pwd", p1->text());
            q.bindValue(":email", email);
            if (q.exec()) {
                pwdDlg->accept();
                QMessageBox::information(this, "Succes",
                    "Mot de passe mis a jour.\nVous pouvez maintenant vous connecter.");
            } else {
                pwdErr->setText("Erreur lors de la mise a jour: " + q.lastError().text());
            }
        });

        pwdDlg->exec();
        delete exp;
        delete attempts;
    });

    dlg->exec();
}
