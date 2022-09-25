#include "Image.h"
#include "StickerSheet.h"

int main() {
  
  Image i;
  i.readFromFile("../alma.png");
  StickerSheet* stickers = new StickerSheet(i, 3);

  i.resize(i.width() / 2, i.height());
  i.illinify();
  i.saturate(0.2);

  stickers->addSticker(i, 0, 0);

  i.readFromFile("../demogorgon.png");
  i.scale(0.5);
  i.illinify();
  i.saturate(0.2);
  stickers->addSticker(i, 0, 117);

  i.readFromFile("../eleven.png");
  i.scale(0.3333);
  stickers->addSticker(i, 587, 361);

  Image stickersheet = stickers->render();
  stickersheet.writeToFile("../myImage.png");
  delete stickers;
  return 0;
}
