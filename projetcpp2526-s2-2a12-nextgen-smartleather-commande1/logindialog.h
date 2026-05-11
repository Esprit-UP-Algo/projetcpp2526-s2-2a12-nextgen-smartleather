#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QMainWindow>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPainter>
#include <QCamera>
#include <QVideoWidget>
#include <QVideoSink>
#include <QVideoFrame>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QMediaCaptureSession>
#include <QImage>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QSettings>

// ═══════════════════════════════════════════════════════════════════════════
// AuthGuard — Gestionnaire de sécurité (blocage progressif + persistance)
// ═══════════════════════════════════════════════════════════════════════════
class AuthGuard
{
public:
    static AuthGuard &instance();

    // Appeler après chaque échec — retourne true si bloqué
    bool recordFailure();
    // Appeler après succès
    void recordSuccess();
    // Vrai si actuellement bloqué
    bool isLocked() const;
    // Secondes restantes avant déblocage (0 si pas bloqué)
    int  secondsRemaining() const;
    // Nombre de tentatives échouées
    int  failedAttempts() const { return m_failed; }
    static constexpr int kMaxAttempts = 3;

private:
    AuthGuard();
    void load();
    void save();
    int  lockDuration() const; // durée selon le niveau de blocage

    int       m_failed   = 0;
    int       m_level    = 0;   // niveau de blocage (0=30s, 1=60s, 2=300s)
    QDateTime m_lockTime;
    bool      m_locked   = false;

    static const int     kDurations[]; // 30s, 60s, 300s
};

// ═══════════════════════════════════════════════════════════════════════════
// FaceEngine
// ═══════════════════════════════════════════════════════════════════════════
class FaceEngine
{
public:
    static bool    enrollAdmin(const QImage &frame);
    static bool    verify(const QImage &frame);
    static bool    hasEnrolled();
    static QString enrollPath();
private:
    static double  compare(const QImage &a, const QImage &b);
    static QImage  normalize(const QImage &img);
    static constexpr double kThreshold = 0.62; // seuil abaissé car NCC est plus précis
    static constexpr int    kSize      = 128;  // résolution plus haute = plus de détails
};

// ═══════════════════════════════════════════════════════════════════════════
// FaceResultWidget
// ═══════════════════════════════════════════════════════════════════════════
class FaceResultWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int angle READ angle WRITE setAngle)
public:
    explicit FaceResultWidget(QWidget *parent = nullptr);
    int  angle() const   { return m_angle; }
    void setAngle(int a) { m_angle = a; update(); }
    void showScanning();
    void showSuccess();
    void showFail();
    void reset();
protected:
    void paintEvent(QPaintEvent *) override;
private:
    int   m_angle = 0;
    int   m_state = 0;
    QPropertyAnimation *m_anim = nullptr;
};

// ═══════════════════════════════════════════════════════════════════════════
// FaceScanPopup
// ═══════════════════════════════════════════════════════════════════════════
class FaceScanPopup : public QDialog
{
    Q_OBJECT
public:
    explicit FaceScanPopup(const QString &username,
                           const QString &mode = "verify",
                           QWidget *parent = nullptr);
    ~FaceScanPopup();
    bool accepted() const { return m_accepted; }
private slots:
    void onNewFrame(const QVideoFrame &frame);
private:
    void startCamera();
    void stopCamera();
    void captureAndProcess();

    QCamera              *m_camera      = nullptr;
    QMediaCaptureSession *m_session     = nullptr;
    QVideoWidget         *m_videoWidget = nullptr;
    QVideoSink           *m_sink        = nullptr;
    FaceResultWidget     *m_result      = nullptr;
    QLabel               *m_statusLabel = nullptr;
    QPushButton          *m_btnScan     = nullptr;
    QTimer               *m_scanTimer   = nullptr;
    QImage                m_lastFrame;
    bool                  m_accepted  = false;
    bool                  m_scanning  = false;
    QString               m_mode;
    QString               m_username;
};

// ═══════════════════════════════════════════════════════════════════════════
// LoginDialog
// ═══════════════════════════════════════════════════════════════════════════
class LoginDialog : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    bool isAccepted() const { return m_accepted; }
    QString loggedEmail() const { return m_loggedEmail; }
private slots:
    void onLogin();
    void onCreateAccount();
    void updateLockUI();
    void onForgotPassword();
private:
    QLineEdit   *m_userEdit;
    QLineEdit   *m_passEdit;
    QLabel      *m_errorLabel;
    QLabel      *m_lockLabel;
    QPushButton *m_btnLogin;
    QPushButton *m_btnFace;
    QTimer      *m_lockTimer = nullptr;
    bool         m_accepted  = false;
    QString      m_loggedEmail;
    void setInputsEnabled(bool enabled);
    bool validateCredentials(const QString &user, const QString &pass);
    bool emailExists(const QString &email) const;
};

#endif // LOGINDIALOG_H
