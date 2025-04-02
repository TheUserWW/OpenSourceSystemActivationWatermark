#include <QApplication>
#include <QWidget>
#include <QScreen>
#include <QPainter>
#include <QFontDatabase>
#include <QLocale>
#include <QDebug>
#include <QTranslator>

class WindowsWatermark : public QWidget {
public:
    explicit WindowsWatermark(QScreen* screen) : QWidget(nullptr) {
        // 初始化多语言文本
        initTranslations();

        // 获取当前系统语言
        QString sysLang = QLocale::system().name().left(2);
        m_text1 = m_translations.value(sysLang + "_line1", "Activate Windows");
        m_text2 = m_translations.value(sysLang + "_line2", "Go to Settings to activate Windows");

        // 窗口设置
        setWindowFlags(Qt::FramelessWindowHint |
                       Qt::WindowStaysOnTopHint |
                       Qt::ToolTip);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_TransparentForMouseEvents);

        // 设置微软雅黑字体
        m_font.setFamily("Microsoft YaHei");
        m_font.setPixelSize(20);
        m_font.setWeight(QFont::Normal);

        // 计算位置
        updatePosition(screen);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setFont(m_font);
        painter.setPen(QColor(120, 120, 120, 160));

        QFontMetrics fm(m_font);
        painter.drawText(0, fm.ascent() + 2, m_text1);
        painter.drawText(0, fm.ascent() + fm.height() + 4, m_text2);
    }

private:
    void initTranslations() {
        // 支持的语言映射表
        m_translations = {
            {"zh_line1", "激活 Windows"},
            {"zh_line2", "转到“设置”以激活 Windows"},
            {"en_line1", "Activate Windows"},
            {"en_line2", "Go to Settings to activate Windows"},
            {"de_line1", "Windows aktivieren"},
            {"de_line2", "Gehen Sie zu Einstellungen, um Windows zu aktivieren"},
            {"ja_line1", "Windows を有効化"},
            {"ja_line2", "設定に移動して Windows を有効化してください"},
            {"fr_line1", "Activer Windows"},
            {"fr_line2", "Accédez aux paramètres pour activer Windows"},
            {"es_line1", "Activar Windows"},
            {"es_line2", "Ve a configuración para activar Windows"},
            {"ru_line1", "Активируйте Windows"},
            {"ru_line2", "Перейдите в раздел \"Параметры\", чтобы активировать Windows"}
        };
    }

    void updatePosition(QScreen* screen) {
        QFontMetrics fm(m_font);
        int textWidth = qMax(fm.horizontalAdvance(m_text1),
                             fm.horizontalAdvance(m_text2));

        setGeometry(
            screen->geometry().right() - textWidth - 50,
            screen->geometry().bottom() - fm.height() * 2 - 80,
            textWidth + 20,
            fm.height() * 2 + 10
            );
    }

    QFont m_font;
    QString m_text1, m_text2;
    QHash<QString, QString> m_translations;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 高DPI支持
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    // 为每个屏幕创建水印
    foreach (QScreen* screen, QApplication::screens()) {
        WindowsWatermark* watermark = new WindowsWatermark(screen);
        watermark->show();
    }

    return app.exec();
}
