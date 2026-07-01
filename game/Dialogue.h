#pragma once
#include "Npc.h"

// Состояние текущего диалога: идёт ли разговор, с каким NPC и на какой
// реплике. Пока диалог открыт (IsOpen() == true), main.cpp не пропускает
// в игрока обычное движение — только "дальше"/"закрыть", как в большинстве
// RPG с диалоговым окном.
class Dialogue
{
public:
    bool IsOpen() const { return npc != nullptr; }

    // Начинает разговор с конкретным NPC с первой реплики.
    void Open(const Npc& npcToTalkTo)
    {
        npc = &npcToTalkTo;
        lineIndex = 0;
    }

    void Close()
    {
        npc = nullptr;
    }

    // Переходит к следующей реплике. Если реплики закончились — сам
    // закрывает диалог, отдельно вызывать Close() после последней не нужно.
    void Advance()
    {
        if (!IsOpen()) return;

        lineIndex++;
        if (lineIndex >= (int)npc->lines.size())
            Close();
    }

    // Текущая реплика и имя говорящего — вызывать только пока IsOpen() == true.
    const std::string& CurrentLine() const  { return npc->lines[lineIndex]; }
    const std::string& SpeakerName() const  { return npc->name; }

    // Последняя ли это реплика — пригодится, чтобы показать другую подсказку
    // ("Enter — закрыть" вместо "Enter — дальше") на последней строке.
    bool IsLastLine() const { return lineIndex == (int)npc->lines.size() - 1; }

private:
    const Npc* npc = nullptr; // nullptr = диалог закрыт, это и есть признак IsOpen()
    int lineIndex = 0;
};
