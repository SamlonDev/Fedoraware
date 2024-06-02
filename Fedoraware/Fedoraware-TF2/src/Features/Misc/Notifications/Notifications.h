#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <vector>
#include <string>
#include "../../Feature.h"

struct Notification_t
{
	std::string m_sText;
	Color_t m_cColor;
	float m_flTime;
};

class CNotifications
{
public:
	CNotifications(int maxNotifySize = 8) : iMaxNotifySize(maxNotifySize) {}
	~CNotifications() {}

	int GetCapacity() const { return iMaxNotifySize; }
	int GetSize() const { return vNotifications.size(); }

	bool Add(const std::string& sText, Color_t cColor = { 255, 255, 255, 255 }, float flLifeTime = Vars::Logging::Lifetime.Value);
	void Draw();
	void Clear();

private:
	std::vector<Notification_t> vNotifications;
	int iMaxNotifySize;
};

ADD_FEATURE(CNotifications, Notifications)

#endif // NOTIFICATIONS_H
