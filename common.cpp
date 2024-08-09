

#include "common.h"
#include "diagramscene.h"
#include <QDebug>

gMode_t gMode = MoveItem;

void setGMode(int m) 
{
  DiagramScene::Mode mm = (DiagramScene::Mode) m;
  switch(mm) {
    case DiagramScene::Mode::InsertItem: gMode = InsertItem; break;
    case DiagramScene::Mode::InsertLine: gMode = InsertLine; break;
    case DiagramScene::Mode::InsertText: gMode = InsertText; break;
    case DiagramScene::Mode::MoveItem: gMode = MoveItem; break;
    case DiagramScene::Mode::InsertTrain: gMode = InsertTrain; break;
    case DiagramScene::Mode::EditingTrain: gMode = EditingTrain; break;
    case DiagramScene::Mode::Simulating: gMode = Simulating; break;
    default:
      qWarning() << "setGMode: Bad DiagramScene::Mode";
  }
}
