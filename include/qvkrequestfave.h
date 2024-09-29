#ifndef QVKREQUESTFAVE_H
#define QVKREQUESTFAVE_H

#include "qvkrequest.h"
#include "qvkuserinfo.h"
#include "qvkgroupinfo.h"
#include "qvkpostinfo.h"

#include "qvkinfofave.h"

/*!
 * \brief Класс для выполнения методов API ВКонтакте для работы со списком закладок
 */
class QVkRequestFave : public QVkRequest
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param[in] token Идентификатор сессии
     * \param[in] parent Родительский объект
     */
    QVkRequestFave(const QByteArray &token = QByteArray(),
                      QObject *parent = nullptr);

    /*!
     * \brief Запрос списка страниц пользователей, добавленных в закладки
     * \param[in] count Количество закладок, которое нужно вернуть
     * \param[in] offset Смещение, необходимое для выборки определенного подмножества закладок
     */
    void requestFaveUserList(unsigned int count = 0, unsigned int offset = 0);

    /*!
     * \brief Запрос списка сообществ, добавленных в закладки
     * \param[in] count Количество закладок, которое нужно вернуть
     * \param[in] offset Смещение, необходимое для выборки определенного подмножества закладок
     */
    void requestFaveGroupList(unsigned int count = 0, unsigned int offset = 0);

    /*!
     * \brief Запрос списка заисей, добавленных в закладки
     * \param[in] count Количество закладок, которое нужно вернуть
     * \param[in] offset Смещение, необходимое для выборки определенного подмножества закладок
     */
    void requestFavePostList(unsigned int count = 0, unsigned int offset = 0);

    /*!
     * \brief Запрос списка клипов, добавленных в закладки
     * \param[in] count Количество закладок, которое нужно вернуть
     * \param[in] offset Смещение, необходимое для выборки определенного подмножества закладок
     */
    void requestFaveClipList(unsigned int count = 0, unsigned int offset = 0);

    /*!
     * \brief Уделение из закладок сообщества
     * \param[in] groupId Идентификатор сообщества, которое следует удалить из закладок
     */
    void requestFaveGroupRemove(long groupId);

    /*!
     * \brief Уделение из закладок страницы пользователя
     * \param[in] userId Идентификатор пользователя, которого следует удалить из закладок
     */
    void requestFaveUserRemove(unsigned int userId);

    /*!
     * \brief Уделение из закладок записи
     * \param[in] ownerId Идентификатор владельца записи, которую следует удалить из закладок
     * \param[in] postId Идентификатор записи на стене, которую следует удалить из закладок
     */
    void requestFavePostRemove(long ownerId,unsigned long postId);

    /*!
     * \brief Уделение клипа из закладок
     * \param[in] ownerId Идентификатор владельца клипа, который следует удалить из закладок
     * \param[in] postId Идентификатор клипа, который следует удалить из закладок
     */
    void requestFaveClipRemove(long ownerId,unsigned long postId);

private slots:

    /*!
     * \brief Получение ответа на запрос API ВКонтакте
     * \param[in] document JSON-схема с ответом сервера
     */
    void receiveFaveUserList(QJsonDocument document);

    /*!
     * \brief Получение ответа на запрос API ВКонтакте
     * \param[in] document JSON-схема с ответом сервера
     */
    void receiveFaveGroupList(QJsonDocument document);

    /*!
     * \brief Получение ответа на запрос API ВКонтакте
     * \param[in] document JSON-схема с ответом сервера
     */
    void receiveFavePostList(QJsonDocument document);

    /*!
     * \brief Получение ответа на запрос API ВКонтакте
     * \param[in] document JSON-схема с ответом сервера
     */
    void receiveFaveClipList(QJsonDocument document);

    /*!
     * \brief Получение ответа на запрос API ВКонтакте
     * \param[in] document JSON-схема с ответом сервера
     */
    void receiveFaveUserRemoved(QJsonDocument document);

    /*!
     * \brief Получение ответа на запрос API ВКонтакте
     * \param[in] document JSON-схема с ответом сервера
     */
    void receiveFaveGroupRemoved(QJsonDocument document);

    /*!
     * \brief Получение ответа на запрос API ВКонтакте
     * \param[in] document JSON-схема с ответом сервера
     */
    void receiveFavePostRemoved(QJsonDocument document);

    /*!
     * \brief Получение ответа на запрос API ВКонтакте
     * \param[in] document JSON-схема с ответом сервера
     */
    void receiveFaveClipRemoved(QJsonDocument document);

signals:

    /*!
     * \brief Список страниц пользователей, добавленных в закладки, успешно получен
     * \param[out] faveUserInfoList Список страниц пользователей, добавленных в закладки
     * \param[out] faveUserTagList Тэги для страниц
     */
    void faveUserListReceived(QList<VkUserInfoFull> faveUserInfoList,
                              QMap<long int, QVkTagList> faveUserTagList);

    /*!
     * \brief Список сообществ, добавленных в закладки, успешно получен
     * \param[out] faveInfoList Список сообществ, добавленных в закладки
     * \param[out] faveGroupTagList Тэги для страниц
     */
    void faveGroupListReceived(QList<VkGroupInfo> faveGroupInfoList,
                               QMap<long int, QVkTagList> faveGroupTagList);

    /*!
     * \brief Список записей, добавленных в закладки, успешно получен
     * \param[out] favePostInfoList Список записей, добавленных в закладки
     * \param[out] favePostTagList Тэги для записей
     */
    void favePostListReceived(QList<VkPostInfo> favePostInfoList,
                               QMap<unsigned long int, QVkTagList> favePostTagList);

    /*!
     * \brief Список клипов, добавленных в закладки, успешно получен
     * \param[out] faveClipInfoList Список клипов, добавленных в закладки
     */
    void faveClipListReceived(QList<VkFaveClipInfo> faveClipInfoList);

    /*!
     * \brief Пользователь удалён из закладок
     */
    void faveUserRemoved();

    /*!
     * \brief Сообщество удалено из закладок
     */
    void faveGroupRemoved();

    /*!
     * \brief Запись удалена из закладок
     */
    void favePostRemoved();

    /*!
     * \brief Клип удалён из закладок
     */
    void faveClipRemoved();
};

#endif // QVKREQUESTFAVE_H
