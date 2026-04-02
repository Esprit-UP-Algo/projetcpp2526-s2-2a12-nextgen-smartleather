#include "logindialog.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>

LoginDialog::LoginDialog(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("NextGen - Leader Factory Management");
    resize(1024, 680);

    // Centrer la fenêtre
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect sg = screen->availableGeometry();
        move(sg.center() - rect().center());
    }

    // Widget central avec fond beige
    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #f7f3ee;");
    setCentralWidget(central);

    // Layout principal centré
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Carte blanche
    QFrame *card = new QFrame(central);
    card->setFixedWidth(860);
    card->setStyleSheet(
        "QFrame {"
        "  background-color: #ffffff;"
        "  border: 1px solid #e0d5cc;"
        "  border-radius: 12px;"
        "}"
    );

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(60, 40, 60, 40);
    cardLayout->setSpacing(14);

    // Titre "Connexion"
    QLabel *title = new QLabel("Connexion", card);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 16px;"
        "font-weight: bold;"
        "color: #2c2c2c;"
        "border: none;"
        "background: transparent;"
    );
    cardLayout->addWidget(title);
    cardLayout->addSpacing(6);

    // Champ utilisateur
    m_userEdit = new QLineEdit(card);
    m_userEdit->setPlaceholderText("Adresse e-mail ou nom d'utilisateur");
    m_userEdit->setFixedHeight(46);
    m_userEdit->setStyleSheet(
        "QLineEdit {"
        "  border: 1.5px solid #C68E65;"
        "  border-radius: 6px;"
        "  padding: 0 14px;"
        "  font-size: 13px;"
        "  color: #2c2c2c;"
        "  background: #ffffff;"
        "}"
        "QLineEdit:focus { border: 2px solid #8B4513; }"
    );
    cardLayout->addWidget(m_userEdit);

    // Champ mot de passe
    m_passEdit = new QLineEdit(card);
    m_passEdit->setPlaceholderText("Mot de passe");
    m_passEdit->setEchoMode(QLineEdit::Password);
    m_passEdit->setFixedHeight(46);
    m_passEdit->setStyleSheet(
        "QLineEdit {"
        "  border: 1.5px solid #d0c8c0;"
        "  border-radius: 6px;"
        "  padding: 0 14px;"
        "  font-size: 13px;"
        "  color: #2c2c2c;"
        "  background: #ffffff;"
        "}"
        "QLineEdit:focus { border: 2px solid #8B4513; }"
    );
    cardLayout->addWidget(m_passEdit);

    // Message d'erreur
    m_errorLabel = new QLabel("", card);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setStyleSheet(
        "color: #c0392b; font-size: 12px; border: none; background: transparent;"
    );
    m_errorLabel->setVisible(false);
    cardLayout->addWidget(m_errorLabel);

    // Bouton Se connecter
    QPushButton *btnLogin = new QPushButton("Se connecter", card);
    btnLogin->setFixedHeight(46);
    btnLogin->setCursor(Qt::PointingHandCursor);
    btnLogin->setStyleSheet(
        "QPushButton {"
        "  background-color: #7B3F00;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 6px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover  { background-color: #8B4513; }"
        "QPushButton:pressed{ background-color: #5a2d00; }"
    );
    cardLayout->addWidget(btnLogin);

    // Lien Créer un compte
    QPushButton *btnCreate = new QPushButton("Créer un compte", card);
    btnCreate->setFlat(true);
    btnCreate->setCursor(Qt::PointingHandCursor);
    btnCreate->setStyleSheet(
        "QPushButton {"
        "  color: #8B4513;"
        "  font-size: 13px;"
        "  text-decoration: underline;"
        "  border: none;"
        "  background: transparent;"
        "}"
        "QPushButton:hover { color: #5a2d00; }"
    );
    cardLayout->addWidget(btnCreate, 0, Qt::AlignCenter);

    mainLayout->addWidget(card);

    connect(btnLogin,  &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(btnCreate, &QPushButton::clicked, this, &LoginDialog::onCreateAccount);
    connect(m_passEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLogin);
    connect(m_userEdit, &QLineEdit::returnPressed, this, [=]{ m_passEdit->setFocus(); });
}

void LoginDialog::onLogin()
{
    const QString user = m_userEdit->text().trimmed();
    const QString pass = m_passEdit->text();

    if (user.isEmpty() || pass.isEmpty()) {
        m_errorLabel->setText("Veuillez remplir tous les champs.");
        m_errorLabel->setVisible(true);
        return;
    }

    if (validateCredentials(user, pass)) {
        m_accepted = true;
        close();
    } else {
        m_errorLabel->setText("Identifiants incorrects. Réessayez.");
        m_errorLabel->setVisible(true);
        m_passEdit->clear();
        m_passEdit->setFocus();
    }
}

void LoginDialog::onCreateAccount()
{
    QMessageBox::information(this, "Créer un compte",
        "Contactez l'administrateur pour créer un compte.\n"
        "Email : admin@smartleather.com");
}

bool LoginDialog::validateCredentials(const QString &user, const QString &pass)
{
    const QList<QPair<QString,QString>> accounts = {
        {"admin",        "admin123"},
        {"manager",      "manager123"},
        {"employe",      "employe123"},
        {"smartleather", "smartleather123"}
    };
    for (const auto &acc : accounts)
        if (acc.first == user && acc.second == pass)
            return true;
    return false;
}
