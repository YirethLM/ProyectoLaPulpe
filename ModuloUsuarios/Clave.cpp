
#include "Clave.h"
#include <QCryptographicHash>
#include <QPasswordDigestor>
#include <QRandomGenerator>
#include <QRegularExpression>

namespace {
    constexpr int iteraciones = 600000;

    bool descomponer(
        const QString& registro,
        QByteArray& sal,
        QByteArray& hash) {

        const auto partes = registro.split('$');

        static const QRegularExpression hexSal(
            "^[0-9a-f]{32}$"
        );

        static const QRegularExpression hexHash(
            "^[0-9a-f]{64}$"
        );

        if (partes.size() != 4 ||
            partes[0] != "pbkdf2-sha256" ||
            partes[1] != QString::number(iteraciones) ||
            !hexSal.match(partes[2]).hasMatch() ||
            !hexHash.match(partes[3]).hasMatch()) {
            return false;
        }

        sal = QByteArray::fromHex(partes[2].toLatin1());
        hash = QByteArray::fromHex(partes[3].toLatin1());
        return true;
    }
}

QString Clave::generar(const QString& texto) {
    QByteArray sal(16, '\0');

    for (int i = 0; i < sal.size(); ++i) {
        sal[i] = static_cast<char>(
            QRandomGenerator::system()->generate() & 255u
            );
    }

    const auto hash = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        texto.toUtf8(),
        sal,
        iteraciones,
        32
    );

    return "pbkdf2-sha256$" +
        QString::number(iteraciones) + "$" +
        QString::fromLatin1(sal.toHex()) + "$" +
        QString::fromLatin1(hash.toHex());
}

bool Clave::formatoValido(const QString& registro) {
    QByteArray sal, hash;
    return descomponer(registro, sal, hash);
}

bool Clave::verificar(
    const QString& texto,
    const QString& registro) {

    QByteArray sal, esperado;

    if (!descomponer(registro, sal, esperado)) {
        return false;
    }

    const auto actual = QPasswordDigestor::deriveKeyPbkdf2(
        QCryptographicHash::Sha256,
        texto.toUtf8(),
        sal,
        iteraciones,
        32
    );

    if (actual.size() != esperado.size()) {
        return false;
    }

    unsigned diferencia = 0;

    for (int i = 0; i < actual.size(); ++i) {
        diferencia |=
            static_cast<unsigned char>(actual[i]) ^
            static_cast<unsigned char>(esperado[i]);
    }

    return diferencia == 0;
}
