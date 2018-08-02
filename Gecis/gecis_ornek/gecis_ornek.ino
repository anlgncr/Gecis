#include <Gecis.h>
//degisken adresi, ilk_deger, son_deger, toplam_zaman, tekrar, ters, fonk
Gecis animasyon(10);

int a;
Gecis::Tween t1;
void setup() {
  Serial.begin(9600);
  t1 = {&a, 0, 1000, 1000, false, true, YUMUSAK_GIRIS};
  animasyon.ekle(&t1);
}

void loop() {
 animasyon.guncelle();
 
 if(!t1.bitti)
 Serial.println(a);

}
