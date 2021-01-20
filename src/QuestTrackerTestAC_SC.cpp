/*
* This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "Log.h"
#include "Chat.h"
#include "QuestTracker.h"
#include "GitRevision.h"

class QuestTrackerTest_Command : public CommandScript
{
public:
    QuestTrackerTest_Command() : CommandScript("QuestTrackerTest_Command") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable = // .commands
        {
            { "questtracker",   SEC_CONSOLE,    false,  &HandleQuestTrackerCommand, ""}
        };

        return commandTable;
    }

    static bool HandleQuestTrackerCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
        {
            handler->SendSysMessage("> Введён пустой аргумент");
            handler->SetSentErrorMessage(true);
            return false;
        }

        ///- Parse the command line arguments
        char* _playerCount = strtok((char*)args, " ");
        char* _trackerType = strtok(nullptr, " ");

        if (!_playerCount || !_trackerType)
        {
            handler->PSendSysMessage("> Неверые аргументы (%s) (%s)", _playerCount, _trackerType);
            handler->SetSentErrorMessage(true);
            return false;
        }

        auto trackerType = atoi(_trackerType);
        auto playerCount = atoi(_playerCount);

        if (!playerCount)
        {
            handler->PSendSysMessage("> Неверый аргументы playerCount (%u)", playerCount);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (trackerType > 3)
        {
            handler->PSendSysMessage("> Неверый аргументы trackerType (%u)", trackerType);
            handler->SetSentErrorMessage(true);
            return false;
        }

        /*
         * Tracker type:
         * 0 - Add
         * 1 - UpdateCompleteTime
         * 2 - UpdateAbandonTime
         * 3 - UpdateGMComplete
         */

        sLog->outString("> QuestTracker: Add queue. Type (%u). Players (%u)", trackerType, playerCount);

        while (playerCount)
        {
            uint32 questID = urand(0, 10000);
            uint32 charID = urand(0, 10000);

            switch (trackerType)
            {
            case 0:
                sQuestTracker->Add(questID, charID, GitRevision::GetHash(), GitRevision::GetDate());
                break;
            case 1:
                sQuestTracker->UpdateCompleteTime(questID, charID);
                break;
            case 2:
                sQuestTracker->UpdateAbandonTime(questID, charID);
                break;
            case 3:
                sQuestTracker->UpdateGMComplete(questID, charID);
                break;
            default:
                break;
            }

            playerCount--;
        }

        return true;
    }
};


// Group all custom scripts
void AddSC_QuestTrackerTest()
{
    new QuestTrackerTest_Command();
}
