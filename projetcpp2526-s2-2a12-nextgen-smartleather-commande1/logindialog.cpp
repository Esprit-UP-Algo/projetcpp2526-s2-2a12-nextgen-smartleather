#include "logindialog.h"
#include "connection.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QInputDialog>
#include <QSettings>
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QDateTime>
#include <QFileDialog>
#include <QImage>
#include <QImageReader>

LoginDialog::LoginDialog(QWidget *parent)
    : QMainWindow(parent)
    , m_lockTimer(new QTimer(this))
{
    setupUI();

    connect(m_btnLogin, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(m_btnFaceIdLogin, &QPushButton::clicked, this, &LoginDialog::onFaceIdLogin);
    connect(m_btnEnrollFaceId, &QPushButton::clicked, this, &LoginDialog::onEnrollFaceId);
    connect(m_btnCreateAccount, &QPushButton::clicked, this, &LoginDialog::onCreateAccount);
    connect(m_btnForgotPassword, &QPushButton::clicked, this, &LoginDialog::onForgotPassword);
    connect(m_lockTimer, &QTimer::timeout, this, &LoginDialog::updateLockUI);

    setWindowTitle("Smart Leather Factory - Login");
    resize(500, 400);

    const QScreen *screen = QApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int x = (screenGeometry.width() - width()) / 2;
    const int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(16);

    QLabel *titleLabel = new QLabel("SMART LEATHER FACTORY", this);
    titleLabel->setStyleSheet(
        "QLabel { color: #3D362D; font-size: 20px; font-weight: 800; text-align: center; }"
    );
    mainLayout->addWidget(titleLabel);

    QLabel *subtitleLabel = new QLabel("Connexion", this);
    subtitleLabel->setStyleSheet(
        "QLabel { color: #8B4513; font-size: 14px; font-weight: 600; text-align: center; }"
    );
    mainLayout->addWidget(subtitleLabel);

    QFrame *formFrame = new QFrame(this);
    formFrame->setStyleSheet(
        "QFrame { border: 1px solid #E5D2BF; border-radius: 10px; background-color: white; }"
    );
    QVBoxLayout *formLayout = new QVBoxLayout(formFrame);
    formLayout->setContentsMargins(20, 20, 20, 20);
    formLayout->setSpacing(12);

    QLabel *emailLabel = new QLabel("Email :", this);
    emailLabel->setStyleSheet("QLabel { color: #4E2C23; font-weight: 600; }");
    formLayout->addWidget(emailLabel);

    m_userEdit = new QLineEdit(this);
    m_userEdit->setPlaceholderText("Entrez votre email");
    m_userEdit->setStyleSheet(
        "QLineEdit { background-color: #FFFCFA; border: 1px solid #DDC6B0; border-radius: 8px; "
        "padding: 10px; font-size: 13px; min-height: 36px; } "
        "QLineEdit:focus { border: 2px solid #C68E65; background-color: white; }"
    );
    formLayout->addWidget(m_userEdit);

    QLabel *passLabel = new QLabel("Mot de passe :", this);
    passLabel->setStyleSheet("QLabel { color: #4E2C23; font-weight: 600; }");
    formLayout->addWidget(passLabel);

    m_passEdit = new QLineEdit(this);
    m_passEdit->setPlaceholderText("Entrez votre mot de passe");
    m_passEdit->setEchoMode(QLineEdit::Password);
    m_passEdit->setStyleSheet(
        "QLineEdit { background-color: #FFFCFA; border: 1px solid #DDC6B0; border-radius: 8px; "
        "padding: 10px; font-size: 13px; min-height: 36px; } "
        "QLineEdit:focus { border: 2px solid #C68E65; background-color: white; }"
    );
    formLayout->addWidget(m_passEdit);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("QLabel { color: #8B0000; font-size: 11px; }");
    m_errorLabel->setWordWrap(true);
    formLayout->addWidget(m_errorLabel);

    m_lockLabel = new QLabel(this);
    m_lockLabel->setStyleSheet("QLabel { color: #FF6B6B; font-size: 11px; font-weight: 600; }");
    m_lockLabel->setWordWrap(true);
    m_lockLabel->setVisible(false);
    formLayout->addWidget(m_lockLabel);

    mainLayout->addWidget(formFrame);

    m_btnLogin = new QPushButton("Se connecter", this);
    m_btnLogin->setStyleSheet(
        "QPushButton { background-color: #8B4513; color: white; border-radius: 8px; "
        "padding: 12px; font-weight: 700; border: none; } "
        "QPushButton:hover { background-color: #4E2C23; } "
        "QPushButton:pressed { background-color: #3D362D; }"
    );
    m_btnLogin->setMinimumHeight(44);
    mainLayout->addWidget(m_btnLogin);

    m_btnFaceIdLogin = new QPushButton("Se connecter avec Face ID", this);
    m_btnFaceIdLogin->setStyleSheet(
        "QPushButton { background-color: #C68E65; color: white; border-radius: 8px; "
        "padding: 10px; font-weight: 700; border: none; } "
        "QPushButton:hover { background-color: #b1784e; } "
        "QPushButton:pressed { background-color: #9c6540; }"
    );
    m_btnFaceIdLogin->setMinimumHeight(42);
    mainLayout->addWidget(m_btnFaceIdLogin);

    m_btnEnrollFaceId = new QPushButton("Enroler Face ID", this);
    m_btnEnrollFaceId->setStyleSheet(
        "QPushButton { background-color: transparent; color: #8B4513; border: 1px solid #C68E65; "
        "border-radius: 8px; padding: 8px; font-weight: 600; } "
        "QPushButton:hover { background-color: #FFF0E0; }"
    );
    m_btnEnrollFaceId->setMinimumHeight(40);
    mainLayout->addWidget(m_btnEnrollFaceId);

    QHBoxLayout *linkLayout = new QHBoxLayout();
    linkLayout->setSpacing(12);

    m_btnCreateAccount = new QPushButton("Creer un compte", this);
    m_btnCreateAccount->setStyleSheet(
        "QPushButton { background-color: transparent; color: #8B4513; border: 1px solid #8B4513; "
        "border-radius: 6px; padding: 8px; font-weight: 600; } "
        "QPushButton:hover { background-color: #FFF0E0; }"
    );
    linkLayout->addWidget(m_btnCreateAccount);

    m_btnForgotPassword = new QPushButton("Mot de passe oublie?", this);
    m_btnForgotPassword->setStyleSheet(
        "QPushButton { background-color: transparent; color: #8B4513; border: none; "
        "padding: 8px; font-weight: 600; text-decoration: underline; } "
        "QPushButton:hover { color: #4E2C23; }"
    );
    linkLayout->addWidget(m_btnForgotPassword);

    linkLayout->addStretch();
    mainLayout->addLayout(linkLayout);

    mainLayout->addStretch();

    m_userEdit->setFocus();
}

void LoginDialog::setInputsEnabled(bool enabled)
{
    m_userEdit->setEnabled(enabled);
    m_passEdit->setEnabled(enabled);
    m_btnLogin->setEnabled(enabled);
    m_btnFaceIdLogin->setEnabled(enabled);
    m_btnEnrollFaceId->setEnabled(enabled);
    m_btnCreateAccount->setEnabled(enabled);
    m_btnForgotPassword->setEnabled(enabled);
}

QString LoginDialog::computeFaceHash(const QString &imagePath) const
{
    QImageReader reader(imagePath);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    if (image.isNull()) {
        return QString();
    }

    QImage gray = image.convertToFormat(QImage::Format_Grayscale8).scaled(9, 8, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QByteArray hashBits;
    hashBits.resize(8);
    hashBits.fill(0);

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            const int left = qGray(gray.pixel(x, y));
            const int right = qGray(gray.pixel(x + 1, y));
            if (left > right) {
                hashBits[y] = static_cast<char>(hashBits[y] | (1 << (7 - x)));
            }
        }
    }

    return QString::fromLatin1(hashBits.toHex());
}

int LoginDialog::hexHammingDistance(const QString &leftHex, const QString &rightHex) const
{
    const QByteArray left = QByteArray::fromHex(leftHex.toLatin1());
    const QByteArray right = QByteArray::fromHex(rightHex.toLatin1());
    if (left.size() != right.size() || left.isEmpty()) {
        return 999;
    }

    int distance = 0;
    for (int i = 0; i < left.size(); ++i) {
        unsigned char v = static_cast<unsigned char>(left.at(i) ^ right.at(i));
        for (int b = 0; b < 8; ++b) {
            if (v & (1u << b)) {
                distance++;
            }
        }
    }
    return distance;
}

bool LoginDialog::validateCredentials(const QString &user, const QString &pass)
{
    if (user.isEmpty() || pass.isEmpty()) {
        m_errorLabel->setText("Email et mot de passe requis.");
        return false;
    }

    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        Connection::instance()->createConnect();
        db = Connection::instance()->database();
    }

    if (!db.isValid() || !db.isOpen()) {
        m_errorLabel->setText("Erreur de connexion a la base de donnees.");
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT PASSWORD FROM UTILISATEUR WHERE EMAIL = :email");
    query.bindValue(":email", user);

    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        m_errorLabel->setText("Erreur d'authentification.");
        return false;
    }

    if (!query.next()) {
        m_errorLabel->setText("Email ou mot de passe incorrect.");
        return false;
    }

    const QString storedPassword = query.value(0).toString();
    if (storedPassword != pass) {
        m_errorLabel->setText("Email ou mot de passe incorrect.");
        return false;
    }

    m_errorLabel->setText("");
    m_loggedEmail = user;
    return true;
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

    if (!query.exec()) {
        return false;
    }

    return query.next();
}

void LoginDialog::updateLockUI()
{
    QSettings settings("SmartLeather", "LoginGuard");
    const int lockTime = settings.value("lockTime", 0).toInt();
    const int remaining = lockTime - QDateTime::currentDateTime().toSecsSinceEpoch();

    if (remaining > 0) {
        m_lockLabel->setText(QString("Compte temporairement verrouille. Reessayez dans %1s.").arg(remaining));
        m_lockLabel->setVisible(true);
        setInputsEnabled(false);
    } else {
        m_lockLabel->setVisible(false);
        setInputsEnabled(true);
        m_lockTimer->stop();
        settings.setValue("failedAttempts", 0);
        m_failedAttempts = 0;
    }
}

void LoginDialog::onLogin()
{
    QSettings settings("SmartLeather", "LoginGuard");
    const int lockTime = settings.value("lockTime", 0).toInt();
    if (QDateTime::currentDateTime().toSecsSinceEpoch() < lockTime) {
        updateLockUI();
        return;
    }

    m_errorLabel->setText("");

    if (validateCredentials(m_userEdit->text(), m_passEdit->text())) {
        m_accepted = true;
        settings.setValue("failedAttempts", 0);
        m_failedAttempts = 0;
        close();
        return;
    }

    m_failedAttempts++;
    settings.setValue("failedAttempts", m_failedAttempts);

    if (m_failedAttempts >= kMaxAttempts) {
        const int lockDuration = 300;
        const int lockTimeVal = QDateTime::currentDateTime().toSecsSinceEpoch() + lockDuration;
        settings.setValue("lockTime", lockTimeVal);
        m_lockLabel->setText("Compte temporairement verrouille pour 5 minutes.");
        m_lockLabel->setVisible(true);
        setInputsEnabled(false);

        m_lockTimer->start(1000);
        updateLockUI();
    }
}

void LoginDialog::onEnrollFaceId()
{
    QString email = m_userEdit->text().trimmed();
    bool ok = true;
    if (email.isEmpty()) {
        email = QInputDialog::getText(this, "Enroler Face ID", "Email du compte :", QLineEdit::Normal, "", &ok).trimmed();
    }
    if (!ok || email.isEmpty()) {
        return;
    }

    if (!emailExists(email)) {
        QMessageBox::warning(this, "Face ID", "Cet email n'existe pas. Creez d'abord le compte.");
        return;
    }

    const QString imagePath = QFileDialog::getOpenFileName(this, "Choisir une photo visage", QString(), "Images (*.png *.jpg *.jpeg *.bmp)");
    if (imagePath.isEmpty()) {
        return;
    }

    const QString faceHash = computeFaceHash(imagePath);
    if (faceHash.isEmpty()) {
        QMessageBox::warning(this, "Face ID", "Impossible de lire l'image selectionnee.");
        return;
    }

    QSettings settings("SmartLeather", "FaceId");
    const QString emailKey = QString::fromLatin1(QCryptographicHash::hash(email.toLower().toUtf8(), QCryptographicHash::Sha256).toHex());
    settings.setValue(QString("users/%1/hash").arg(emailKey), faceHash);
    settings.setValue(QString("users/%1/email").arg(emailKey), email);

    m_userEdit->setText(email);
    QMessageBox::information(this, "Face ID", "Face ID enregistre avec succes pour cet email.");
}

void LoginDialog::onFaceIdLogin()
{
    QSettings guardSettings("SmartLeather", "LoginGuard");
    const int lockTime = guardSettings.value("lockTime", 0).toInt();
    if (QDateTime::currentDateTime().toSecsSinceEpoch() < lockTime) {
        updateLockUI();
        return;
    }

    QString email = m_userEdit->text().trimmed();
    bool ok = true;
    if (email.isEmpty()) {
        email = QInputDialog::getText(this, "Face ID", "Email :", QLineEdit::Normal, "", &ok).trimmed();
    }
    if (!ok || email.isEmpty()) {
        m_errorLabel->setText("Email requis pour Face ID.");
        return;
    }

    QSettings faceSettings("SmartLeather", "FaceId");
    const QString emailKey = QString::fromLatin1(QCryptographicHash::hash(email.toLower().toUtf8(), QCryptographicHash::Sha256).toHex());
    const QString storedHash = faceSettings.value(QString("users/%1/hash").arg(emailKey)).toString();
    if (storedHash.isEmpty()) {
        m_errorLabel->setText("Face ID non enregistre pour cet email.");
        return;
    }

    const QString imagePath = QFileDialog::getOpenFileName(this, "Choisir la photo Face ID", QString(), "Images (*.png *.jpg *.jpeg *.bmp)");
    if (imagePath.isEmpty()) {
        return;
    }

    const QString candidateHash = computeFaceHash(imagePath);
    if (candidateHash.isEmpty()) {
        m_errorLabel->setText("Image invalide pour Face ID.");
        return;
    }

    const int distance = hexHammingDistance(storedHash, candidateHash);
    if (distance > kFaceHashThreshold) {
        m_errorLabel->setText("Face ID non reconnu.");
        m_failedAttempts++;
        guardSettings.setValue("failedAttempts", m_failedAttempts);
        if (m_failedAttempts >= kMaxAttempts) {
            const int lockTimeVal = QDateTime::currentDateTime().toSecsSinceEpoch() + 300;
            guardSettings.setValue("lockTime", lockTimeVal);
            m_lockTimer->start(1000);
            updateLockUI();
        }
        return;
    }

    m_errorLabel->setText("");
    m_loggedEmail = email;
    m_accepted = true;
    guardSettings.setValue("failedAttempts", 0);
    m_failedAttempts = 0;
    close();
}

void LoginDialog::onCreateAccount()
{
    bool ok;
    QString email = QInputDialog::getText(
        this, "Creer un compte", "Email :", QLineEdit::Normal, "", &ok
    );
    if (!ok || email.isEmpty()) {
        return;
    }

    if (emailExists(email)) {
        QMessageBox::warning(this, "Erreur", "Cet email est deja enregistre.");
        return;
    }

    QString password = QInputDialog::getText(
        this, "Creer un compte", "Mot de passe :", QLineEdit::Password, "", &ok
    );
    if (!ok || password.isEmpty()) {
        return;
    }

    QString confirmPassword = QInputDialog::getText(
        this, "Creer un compte", "Confirmer le mot de passe :", QLineEdit::Password, "", &ok
    );
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

void LoginDialog::onForgotPassword()
{
    bool ok;
    QString email = QInputDialog::getText(
        this, "Reinitialiser le mot de passe", "Email :", QLineEdit::Normal, "", &ok
    );
    if (!ok || email.isEmpty()) {
        return;
    }

    if (!emailExists(email)) {
        QMessageBox::warning(this, "Erreur", "Cet email n'est pas enregistre.");
        return;
    }

    QString newPassword = QInputDialog::getText(
        this, "Reinitialiser le mot de passe", "Nouveau mot de passe :", QLineEdit::Password, "", &ok
    );
    if (!ok || newPassword.isEmpty()) {
        return;
    }

    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        Connection::instance()->createConnect();
        db = Connection::instance()->database();
    }

    QSqlQuery query(db);
    query.prepare("UPDATE UTILISATEUR SET PASSWORD = :password WHERE EMAIL = :email");
    query.bindValue(":password", newPassword);
    query.bindValue(":email", email);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de reinitialiser le mot de passe.");
        return;
    }

    QMessageBox::information(this, "Succes", "Mot de passe reinitialise avec succes.");
    m_userEdit->setText(email);
    m_passEdit->clear();
    m_userEdit->setFocus();
}
