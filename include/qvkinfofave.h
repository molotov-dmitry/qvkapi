#ifndef QVKINFOFAVE_H
#define QVKINFOFAVE_H

#include <QPair>
#include <QList>
#include <QString>
#include <QDateTime>

using QVkTagInfo = QPair<unsigned int, QString>;
using QVkTagList = QList<QVkTagInfo>;

struct VkFaveClipInfo
{
    unsigned long       id;             ///< Идентификатор видеозаписи
    long                ownerId;        ///< Идентификатор владельца видеозаписи

    QString             playerUrl;      ///< URL страницы с плеером, который можно использовать для воспроизведения ролика в браузере

    unsigned int        width;          ///< Ширина видео
    unsigned int        height;         ///< Высота видео

    QString             title;          ///< Название видеозаписи
    QString             description;    ///< Текст описания видеозаписи
    long                duration;       ///< Длительность ролика в секундах

    QDateTime           created;        ///< Дата создания видеозаписи
    QDateTime           uploaded;       ///< Дата добавления видеозаписи пользователем или группой

    QVkTagList          tags;           ///< Тэги клипа
};

#endif // QVKINFOFAVE_H
