#include <KDE/KDebug>
#include <QtTest/QtTest>
#include <QtCrypto>

class EncryptTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testCrypo();
}; // End of class EncryptTest.

void
EncryptTest::testCrypo()
{
    QCA::Initializer init;

    QCA::SecureArray arg = "testakiircclient0x0x0x0x0x0x";
    if (!QCA::isSupported("blowfish-cfb")) {
        qDebug() << "AES128-CBC-PLCS7 Not supported";
    }

    QCA::SymmetricKey key(16);
    QCA::InitializationVector iv(16);

    QCA::Cipher cipher(QString("aes128"), QCA::Cipher::CFB, QCA::Cipher::NoPadding,
                       QCA::Encode, key, iv);

    QCA::SecureArray u = cipher.update(arg);
    if (!cipher.ok()) {
        qDebug() << "Update failed";
    }

    printf("AES128 encryption of %s is [%s]\n",
           arg.data(),
           qPrintable(QCA::arrayToHex(u.toByteArray())) );

    QCA::SecureArray f = cipher.final();

    printf("Final block [0x%s]\n", qPrintable(QCA::arrayToHex(f.toByteArray())));

    cipher.setup(QCA::Decode, key, iv);
    QCA::SecureArray cipherText = u.append(f);
    QCA::SecureArray plainText = cipher.update(cipherText);

    if (!cipher.ok()) {
        qDebug() << "Failed";
    }

    printf("Decryption using AES128 of [0x%s] is %s\n",
           qPrintable(QCA::arrayToHex(cipherText.toByteArray())), plainText.data());

    plainText = cipher.final();

    // check if the final() call worked
    if (!cipher.ok()) {
        printf("Final failed\n");
    }

    // output results
    printf("Final decryption block using AES128 is %s\n", plainText.data());
    // instead of update() and final(), you can do the whole thing
    // in one step, using process()
    printf("One step decryption using AES128: %s\n",
           QCA::SecureArray(cipher.process(cipherText)).data() );
}

QTEST_MAIN(EncryptTest)
#include "encrypttest.moc"