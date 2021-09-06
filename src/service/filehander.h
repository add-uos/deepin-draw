#ifndef FILEHANDER_H
#define FILEHANDER_H

#include <QObject>
#include <QImage>
#include <sitemdata.h>
#include <QFuture>

class PageContext;
class FileHander: public QObject
{
    Q_OBJECT
public:
    enum ErrorType {NoError = 0,

                    EFileNameIllegal = 1000, EFileNotExist,

                    //LOAD EEROR TYPE
                    EUnReadableFile, EUnSupportFile, EUnKnowedDdfVersion, EExcessiveDdfVersion,
                    EDdfFileMD5Error, EUserCancelLoad_OldPen, EUserCancelLoad_OldBlur,
                    EDamagedImageFile,

                    //WRITE EEROR TYPE
                    EUnWritableFile, EUnWritableDir, EInsufficientPartitionSpace,
                   };
    FileHander(QObject *parent = nullptr);
    ~FileHander() override;

    /**
     * @brief supPictureSuffix 返回支持的所有图片后缀名
     */
    static QStringList supPictureSuffix();

    /**
     * @brief supDdfStuffix 返回支持的所有ddf后缀名(暂时只有.ddf)
     */
    static QStringList supDdfStuffix();

    static bool    isLegalFile(const QString &file);
    static QString toLegalFile(const QString &file);

    PageContext *loadDdf(const QString &file);
    bool saveToDdf(PageContext *context, const QString &file = "");

    QImage loadImage(const QString &file);
    bool   saveToImage(PageContext *context,
                       const QString &file = "",
                       int imageQuility = 100);

    QString lastErrorDescribe()const;
    int lastError()const;

    bool isVolumeSpaceAvailabel(const QString &desFile, const int needBytesSize);
    EDdfVersion  getDdfVersion(const QString &file) const;
    bool isDdfFileDirty(const QString &filePath) const;


private:
    bool checkFileBeforeLoad(const QString &file, bool isDdf = true);
    bool checkFileBeforeSave(const QString &file, bool toDdf = true);

    bool checkFileExist(const QString &file)const;
    bool checkFileReadable(const QString &file)const;
    bool checkFileWritable(const QString &file)const;

    bool checkDdfVersionIllegal(const QString &ddfFile)const;
    bool checkDdfMd5(const QString &ddfFile)const;

signals:
    void progressBegin(const QString &describe);
    void progressChanged(int progress, int total, const QString &describe);
    void progressEnd(int ret, const QString &describe);
private:
    DECLAREPRIVATECLASS(FileHander)
};

#endif // FILEHANDER_H
