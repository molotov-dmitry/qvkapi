#include "qvkrequestfave.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

QVkRequestFave::QVkRequestFave(const QByteArray& token, QObject* parent)
    : QVkRequest(token, parent, "5.131")
{

}

void QVkRequestFave::requestFaveUserList(unsigned int count, unsigned int offset)
{
    disconnect(this, nullptr, this, nullptr);

    //// Список параметров =====================================================

    QParam paramList;

    //// Идентификатор пользователя --------------------------------------------

    if (count > 0)
    {
        paramList.insert("count", QString::number(count));
    }

    paramList.insert("offset", QString::number(offset));

    //// Тип -------------------------------------------------------------------

    paramList.insert("type", "users");

    //// Список полей ----------------------------------------------------------

    QStringList fields;
    fields << "screen_name";
    fields << "bdate" << "sex" << "online" << "status" << "last_seen" << "verified";
    fields << "photo_50" << "photo_100" << "photo_200_orig" << "photo_200" << "photo_400_orig" << "photo_max" << "photo_max_orig";

    QString fieldsList = fields.join(',');

    paramList.insert("fields", fieldsList);

    //// =======================================================================

    connect(this, &QVkRequest::replySuccess,
            this, &QVkRequestFave::receiveFaveUserList);

    sendRequest("fave.getPages", paramList);
}

void QVkRequestFave::requestFaveGroupList(unsigned int count, unsigned int offset)
{
    disconnect(this, nullptr, this, nullptr);

    //// Список параметров =====================================================

    QParam paramList;

    //// Идентификатор пользователя --------------------------------------------

    if (count > 0)
    {
        paramList.insert("count", QString::number(count));
    }

    paramList.insert("offset", QString::number(offset));

    //// Тип -------------------------------------------------------------------

    paramList.insert("type", "groups");

    //// =======================================================================

    connect(this, &QVkRequest::replySuccess,
            this, &QVkRequestFave::receiveFaveGroupList);

    sendRequest("fave.getPages", paramList);
}

void QVkRequestFave::requestFavePostList(unsigned int count, unsigned int offset)
{
    disconnect(this, nullptr, this, nullptr);

    //// Список параметров =====================================================

    QParam paramList;

    //// Идентификатор пользователя --------------------------------------------

    if (count > 0)
    {
        paramList.insert("count", QString::number(count));
    }

    paramList.insert("offset", QString::number(offset));

    //// Тип -------------------------------------------------------------------

    paramList.insert("item_type", "post");

    //// =======================================================================

    connect(this, &QVkRequest::replySuccess,
            this, &QVkRequestFave::receiveFavePostList);

    sendRequest("fave.get", paramList);
}

void QVkRequestFave::requestFaveClipList(unsigned int count, unsigned int offset)
{
    disconnect(this, nullptr, this, nullptr);

    //// Список параметров =====================================================

    QParam paramList;

    //// Идентификатор пользователя --------------------------------------------

    if (count > 0)
    {
        paramList.insert("count", QString::number(count));
    }

    paramList.insert("offset", QString::number(offset));

    //// Тип -------------------------------------------------------------------

    paramList.insert("item_type", "clip");

    //// =======================================================================

    connect(this, &QVkRequest::replySuccess,
            this, &QVkRequestFave::receiveFaveClipList);

    sendRequest("fave.get", paramList);
}

void QVkRequestFave::requestFaveGroupRemove(long groupId)
{
    disconnect(this, nullptr, this, nullptr);

    //// Список параметров =====================================================

    QParam paramList;

    //// Идентификатор сообщества ----------------------------------------------

    paramList.insert("group_id", QString::number(groupId));

    //// =======================================================================

    connect(this, &QVkRequest::replySuccess,
            this, &QVkRequestFave::receiveFaveGroupRemoved);

    sendRequest("fave.removePage", paramList);
}

void QVkRequestFave::requestFaveUserRemove(unsigned int userId)
{
    disconnect(this, nullptr, this, nullptr);

    //// Список параметров =====================================================

    QParam paramList;

    //// Идентификатор сообщества ----------------------------------------------

    paramList.insert("user_id", QString::number(userId));

    //// =======================================================================

    connect(this, &QVkRequest::replySuccess,
            this, &QVkRequestFave::receiveFaveUserRemoved);

    sendRequest("fave.removePage", paramList);
}

void QVkRequestFave::requestFavePostRemove(long ownerId, unsigned long postId)
{
    disconnect(this, nullptr, this, nullptr);

    //// Список параметров =====================================================

    QParam paramList;

    //// Идентификатор сообщества ----------------------------------------------

    paramList.insert("owner_id", QString::number(ownerId));
    paramList.insert("id", QString::number(postId));

    //// =======================================================================

    connect(this, &QVkRequest::replySuccess,
            this, &QVkRequestFave::receiveFavePostRemoved);

    sendRequest("fave.removePost", paramList);
}

void QVkRequestFave::requestFaveClipRemove(long ownerId, unsigned long postId)
{
    disconnect(this, nullptr, this, nullptr);

    //// Список параметров =====================================================

    QParam paramList;

    //// Идентификатор сообщества ----------------------------------------------

    paramList.insert("owner_id", QString::number(ownerId));
    paramList.insert("id", QString::number(postId));

    //// =======================================================================

    connect(this, &QVkRequest::replySuccess,
            this, &QVkRequestFave::receiveFaveClipRemoved);

    sendRequest("fave.removeClip", paramList);
}

void QVkRequestFave::receiveFaveUserList(QJsonDocument document)
{
    QList<VkUserInfoFull> userInfoList;
    QMap<long int, QVkTagList> tags;

    QJsonObject response = document.object()["response"].toObject();

    //// Iterate trough list ===================================================

    QJsonArray items = response["items"].toArray();

    foreach (const QJsonValue &value, items)
    {
        QJsonObject faveObject   = value.toObject();
        QJsonObject object       = faveObject["user"].toObject();
        QJsonArray  tagsArray    = faveObject["tags"].toArray();
        VkUserInfoFull userInfo;

        //// Базовая информаци о пользователе ==================================

        //// Get user id and name ----------------------------------------------

        userInfo.basic.id = object["id"].toVariant().toUInt();
        userInfo.basic.firstName = object["first_name"].toString();
        userInfo.basic.lastName = object["last_name"].toString();

        //// Get page status ---------------------------------------------------

        if (object.contains("deactivated"))
        {
            QString pageStatus = object["deactivated"].toString();

            if (pageStatus == QString("deleted"))
                userInfo.basic.pageStatus = VkUser::USER_DELETED;
            else if (pageStatus == QString("banned"))
                userInfo.basic.pageStatus = VkUser::USER_BANNED;
            else
                userInfo.basic.pageStatus = VkUser::USER_ACTIVE;
        }
        else
        {
            userInfo.basic.pageStatus = VkUser::USER_ACTIVE;
        }

        //// Get user visibility -----------------------------------------------

        userInfo.basic.userVisibility = VkUser::USER_VISIBLE;

        if (object.contains("hidden"))
        {
            int userHidden= object["hidden"].toInt();

            if (userHidden)
                userInfo.basic.userVisibility = VkUser::USER_HIDDEN;
        }

        if (userInfo.basic.pageStatus != VkUser::USER_ACTIVE || userInfo.basic.userVisibility != VkUser::USER_VISIBLE)
        {
            userInfoList.append(userInfo);
            continue;
        }

        //// Информаци о статусе пользователя ==================================

        //// Короткое имя страницы ---------------------------------------------

        if (object.contains("screen_name"))
            userInfo.status.screenName = object["screen_name"].toString();

        //// Дата рождения -----------------------------------------------------

        QString bDate = object["bdate"].toString();
        QStringList bDateList = bDate.split('.');

        if (bDateList.count() > 0)
            userInfo.status.birthDay = bDateList.at(0).toUShort();

        if (bDateList.count() > 1)
            userInfo.status.birthMonth = bDateList.at(1).toUShort();

        if (bDateList.count() > 2)
            userInfo.status.birthYear = bDateList.at(2).toUShort();

        //// Пол пользователя --------------------------------------------------

        userInfo.status.userSex = (VkUser::Sex)object["sex"].toInt();

        //// Онлайн пользователя -----------------------------------------------

        userInfo.status.userOnline = (VkUser::Online)object["online"].toInt();

        if (userInfo.status.userOnline != VkUser::USER_OFFLINE)
        {
            if (object.contains("online_mobile") && object["online_mobile"].toInt() != 0)
                userInfo.status.userOnline = VkUser::USER_ONLINE_MOBILE;
        }

        //// Статус пользователя -----------------------------------------------

        userInfo.status.statusText = object["status"].toString();

        //// Время последнего посещения ----------------------------------------

        if (object.contains("last_seen"))
        {
            QJsonObject lastSeenObject = object["last_seen"].toObject();

            userInfo.status.lastSeen = QDateTime::fromTime_t(lastSeenObject["time"].toVariant().toUInt());
        }

        //// Сраница верифицирована --------------------------------------------

        userInfo.status.verified = object["verified"].toInt() != 0;

        //// Информация о контактах пользователя ===============================

        //// Город -------------------------------------------------------------

        if (object.contains("city"))
        {
            QJsonObject cityObject = object["city"].toObject();

            userInfo.contacts.city = cityObject["title"].toString();
        }

        //// Страна ------------------------------------------------------------

        if (object.contains("country"))
        {
            QJsonObject countryObject = object["country"].toObject();

            userInfo.contacts.country = countryObject["title"].toString();
        }

        //// Родной город пользователя -----------------------------------------

        if (object.contains("home_town"))
        {
            userInfo.contacts.homeTown = object["home_town"].toString();
        }

        //// Номер мобильного телефона -----------------------------------------

        if (object.contains("mobile_phone"))
            userInfo.contacts.mobilePhone = object["mobile_phone"].toString();


        //// Дополнительный номер телефона -------------------------------------

        if (object.contains("home_phone"))
            userInfo.contacts.homePhone = object["home_phone"].toString();

        //// Сайт --------------------------------------------------------------

        if (object.contains("site"))
            userInfo.contacts.siteUrl = object["site"].toString();

        //// Skype -------------------------------------------------------------

        if (object.contains("skype"))
            userInfo.contacts.skype = object["skype"].toString();

        //// Facebook ----------------------------------------------------------

        if (object.contains("facebook"))
            userInfo.contacts.facebookId = object["facebook"].toString();

        if (object.contains("facebook_name"))
            userInfo.contacts.facebookName = object["facebook_name"].toString();

        //// Twitter -----------------------------------------------------------

        if (object.contains("twitter"))
            userInfo.contacts.twitter = object["twitter"].toString();

        //// Instagram ---------------------------------------------------------

        if (object.contains("instagram"))
            userInfo.contacts.instagram = object["instagram"].toString();


        //// Счётчики ==========================================================

        if (object.contains("counters"))
        {
            QJsonObject countersObject = object["counters"].toObject();

            userInfo.counters.albums = countersObject["albums"].toVariant().toUInt();
            userInfo.counters.videos = countersObject["videos"].toVariant().toUInt();
            userInfo.counters.audios = countersObject["audios"].toVariant().toUInt();
            userInfo.counters.notes = countersObject["notes"].toVariant().toUInt();
            userInfo.counters.photos = countersObject["photos"].toVariant().toUInt();
            userInfo.counters.groups = countersObject["groups"].toVariant().toUInt();
            userInfo.counters.gifts = countersObject["gifts"].toVariant().toUInt();
            userInfo.counters.friends = countersObject["friends"].toVariant().toUInt();
            userInfo.counters.online_friends = countersObject["online_friends"].toVariant().toUInt();
            userInfo.counters.mutual_friends = countersObject["mutual_friends"].toVariant().toUInt();
            userInfo.counters.user_photos = countersObject["user_photos"].toVariant().toUInt();
            userInfo.counters.user_videos = countersObject["user_videos"].toVariant().toUInt();
            userInfo.counters.followers = countersObject["followers"].toVariant().toUInt();
            userInfo.counters.subscriptions = countersObject["subscriptions"].toVariant().toUInt();
            userInfo.counters.pages = countersObject["pages"].toVariant().toUInt();
        }
        else
        {
            userInfo.counters.clear();
        }

        //// Фотографии профиля ================================================

        userInfo.photo.photo_50 = object["photo_50"].toString();
        userInfo.photo.photo_100 = object["photo_100"].toString();
        userInfo.photo.photo_200_orig = object["photo_200_orig"].toString();
        userInfo.photo.photo_200 = object["photo_200"].toString();
        userInfo.photo.photo_400_orig = object["photo_400_orig"].toString();
        userInfo.photo.photo_max = object["photo_max"].toString();
        userInfo.photo.photo_max_orig = object["photo_max_orig"].toString();

        //// Add user info to list =============================================

        userInfoList.append(userInfo);

        //// Tag list ==========================================================

        QVkTagList groupTags;

        foreach (const QJsonValue &tag, tagsArray)
        {
            QVkTagInfo tagInfo;
            tagInfo.first = tag.toObject()["id"].toVariant().toUInt();
            tagInfo.second = tag.toObject()["name"].toString();

            groupTags.append(tagInfo);
        }

        if (not groupTags.isEmpty())
        {
            tags.insert(userInfo.basic.id, groupTags);
        }
    }

    emit faveUserListReceived(userInfoList, tags);
}

void QVkRequestFave::receiveFaveGroupList(QJsonDocument document)
{
    QList<VkGroupInfo> groupInfoList;
    QMap<long int, QVkTagList> tags;

    QJsonObject response = document.object()["response"].toObject();

    //// Iterate trough list ===================================================

    QJsonArray items = response["items"].toArray();

    foreach (const QJsonValue &value, items)
    {
        QJsonObject faveObject   = value.toObject();
        QJsonObject object       = faveObject["group"].toObject();
        QJsonArray  tagsArray    = faveObject["tags"].toArray();
        VkGroupInfo groupInfo;

        groupInfo.id = (long)object["id"].toVariant().toULongLong();
        groupInfo.name = object["name"].toString();
        groupInfo.screenName = object["screen_name"].toString();

        groupInfo.access = (VkGroup::Access)object["is_closed"].toInt();

        if (object["deactivated"].toString() == "deleted")
        {
            groupInfo.status = VkGroup::GROUP_DELETED;
        }
        else if (object["deactivated"].toString() == "banned")
        {
            groupInfo.status = VkGroup::GROUP_BANNED;
        }
        else
        {
            groupInfo.status = VkGroup::GROUP_ACTIVE;
        }

        if (object["type"].toString() == "group")
        {
            groupInfo.type = VkGroup::GROUP_GROUP;
        }
        else if (object["type"].toString() == "page")
        {
            groupInfo.type = VkGroup::GROUP_PAGE;
        }
        else if (object["type"].toString() == "event")
        {
            groupInfo.type = VkGroup::GROUP_EVENT;
        }
        else
        {
            groupInfo.type = VkGroup::GROUP_GROUP;
        }

        groupInfo.photo_50  = object["photo_50"].toString();
        groupInfo.photo_100 = object["photo_100"].toString();
        groupInfo.photo_200 = object["photo_200"].toString();

        groupInfoList.append(groupInfo);

        //// Tag list ==========================================================

        QVkTagList groupTags;

        foreach (const QJsonValue &tag, tagsArray)
        {
            QVkTagInfo tagInfo;
            tagInfo.first = tag.toObject()["id"].toVariant().toUInt();
            tagInfo.second = tag.toObject()["name"].toString();

            groupTags.append(tagInfo);
        }

        if (not groupTags.isEmpty())
        {
            tags.insert(groupInfo.id, groupTags);
        }

    }

    //// =======================================================================

    emit faveGroupListReceived(groupInfoList, tags);
}

void QVkRequestFave::receiveFavePostList(QJsonDocument document)
{
    QList<VkPostInfo> postInfoList;
    QMap<unsigned long int, QVkTagList> tags;

    QJsonObject response = document.object()["response"].toObject();

    //// Iterate trough list ===================================================

    QJsonArray items = response["items"].toArray();

    foreach (const QJsonValue &value, items)
    {
        QJsonObject faveObject   = value.toObject();
        QJsonObject object       = faveObject["post"].toObject();
        QJsonArray  tagsArray    = faveObject["tags"].toArray();

        VkPostInfo  postInfo;

        postInfo.id = object["id"].toVariant().toULongLong();
        postInfo.fromId = object["from_id"].toVariant().toLongLong();
        postInfo.ownerId = object["owner_id"].toVariant().toLongLong();

        postInfo.text = object["text"].toString();

        if (object.contains("date"))
        {
            uint timestamp = object["date"].toVariant().toUInt();
            postInfo.date = QDateTime::fromTime_t(timestamp);
        }

        //// Attachments =======================================================

        QJsonArray attachItems = object["attachments"].toArray();

        foreach (const QJsonValue &attachValue, attachItems)
        {
            QJsonObject attachmentObject = attachValue.toObject();

            //// Photos --------------------------------------------------------

            QString attachmentType = attachmentObject["type"].toString();

            if (attachmentType == "photo")
            {
                VkPostPhotoInfo photoInfo;

                QJsonObject photoObject = attachmentObject["photo"].toObject();

                photoInfo.id      = photoObject["id"].toVariant().toULongLong();
                photoInfo.albumId = photoObject["album_id"].toVariant().toLongLong();
                photoInfo.userId  = photoObject["owner_id"].toVariant().toLongLong();

                QJsonArray sizeItems = photoObject["sizes"].toArray();

                foreach (const QJsonValue &sizeValue, sizeItems)
                {
                    QJsonObject sizeObject = sizeValue.toObject();

                    VkPostPhotoSizeInfo photoSizeInfo;

                    photoSizeInfo.height = sizeObject["height"].toVariant().toUInt();
                    photoSizeInfo.width  = sizeObject["width"].toVariant().toUInt();
                    photoSizeInfo.url    = sizeObject["url"].toString();

                    photoInfo.sizes.append(photoSizeInfo);
                }

                postInfo.photos.append(photoInfo);
            }
            else if (attachmentType == "video")
            {
                VkPostVideoInfo videoInfo;

                QJsonObject videoObject = attachmentObject["video"].toObject();

                videoInfo.id      = videoObject["id"].toVariant().toULongLong();
                videoInfo.ownerId = videoObject["owner_id"].toVariant().toLongLong();

                videoInfo.playerUrl = videoObject["player"].toString();

                videoInfo.width = object["width"].toVariant().toUInt();
                videoInfo.height = object["height"].toVariant().toUInt();

                videoInfo.title = videoObject["title"].toString();
                videoInfo.description = videoObject["description"].toString();
                videoInfo.duration = videoObject["duration"].toVariant().toLongLong();

                if (object.contains("date"))
                {
                    uint timestamp = object["date"].toVariant().toUInt();
                    videoInfo.created = QDateTime::fromTime_t(timestamp);
                }

                if (object.contains("adding_date"))
                {
                    uint timestamp = object["adding_date"].toVariant().toUInt();
                    videoInfo.uploaded = QDateTime::fromTime_t(timestamp);
                }

                postInfo.videos.append(videoInfo);
            }
        }

        //// ===================================================================

        postInfoList.append(postInfo);

        //// Tag list ==========================================================

        QVkTagList groupTags;

        foreach (const QJsonValue &tag, tagsArray)
        {
            QVkTagInfo tagInfo;
            tagInfo.first = tag.toObject()["id"].toVariant().toUInt();
            tagInfo.second = tag.toObject()["name"].toString();

            groupTags.append(tagInfo);
        }

        if (not groupTags.isEmpty())
        {
            tags.insert(postInfo.id, groupTags);
        }
    }

    //// =======================================================================

    emit favePostListReceived(postInfoList, tags);
}

void QVkRequestFave::receiveFaveClipList(QJsonDocument document)
{
    QList<VkFaveClipInfo> clipInfoList;

    QJsonObject response = document.object()["response"].toObject();

    //// Iterate trough list ===================================================

    QJsonArray items = response["items"].toArray();

    foreach (const QJsonValue &value, items)
    {
        QJsonObject faveObject   = value.toObject();
        QJsonObject object       = faveObject["clip"].toObject();
        QJsonArray  tagsArray    = faveObject["tags"].toArray();

        QString attachmentType = faveObject["type"].toString();

        if (attachmentType == "clip")
        {
            VkFaveClipInfo clipInfo;

            QJsonObject videoObject = faveObject["clip"].toObject();

            clipInfo.id      = videoObject["id"].toVariant().toULongLong();
            clipInfo.ownerId = videoObject["owner_id"].toVariant().toLongLong();

            clipInfo.playerUrl = videoObject["player"].toString();

            clipInfo.width = object["width"].toVariant().toUInt();
            clipInfo.height = object["height"].toVariant().toUInt();

            clipInfo.title = videoObject["title"].toString();
            clipInfo.description = videoObject["description"].toString();
            clipInfo.duration = videoObject["duration"].toVariant().toLongLong();

            if (object.contains("date"))
            {
                uint timestamp = object["date"].toVariant().toUInt();
                clipInfo.created = QDateTime::fromTime_t(timestamp);
            }

            if (object.contains("adding_date"))
            {
                uint timestamp = object["adding_date"].toVariant().toUInt();
                clipInfo.uploaded = QDateTime::fromTime_t(timestamp);
            }

            //// Tags ----------------------------------------------------------

            foreach (const QJsonValue &tag, tagsArray)
            {
                QVkTagInfo tagInfo;
                tagInfo.first = tag.toObject()["id"].toVariant().toUInt();
                tagInfo.second = tag.toObject()["name"].toString();

                clipInfo.tags.append(tagInfo);
            }

            //// ---------------------------------------------------------------

            clipInfoList.append(clipInfo);
        }
    }

    //// =======================================================================

    emit faveClipListReceived(clipInfoList);
}

void QVkRequestFave::receiveFaveUserRemoved(QJsonDocument document)
{
    (void)document;

    emit faveUserRemoved();
}

void QVkRequestFave::receiveFaveGroupRemoved(QJsonDocument document)
{
    (void)document;

    emit faveGroupRemoved();
}

void QVkRequestFave::receiveFavePostRemoved(QJsonDocument document)
{
    (void)document;

    emit favePostRemoved();
}

void QVkRequestFave::receiveFaveClipRemoved(QJsonDocument document)
{
    (void)document;

    emit faveClipRemoved();
}
