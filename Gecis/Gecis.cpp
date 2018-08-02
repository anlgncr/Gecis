/*Bu kütüphane ne işe yarar ?

Programımızda integer türünde bir değişkenimiz olduğunu varsayalım.
Biz bu değişkenin değerini 5 saniye içerisinde 0'dan 100'e çıkarmak istiyoruz.
Kütüphane sayesinde değişkenimiz 5 saniye içerisinde artarak 5. saniyede istediğimiz değer olan 100 değerine sahip oluyor.
Ayrıca aynı anda başka bir değişkenin değerini bu şekilde değiştirebiliriz.
-Bir örnekle bunu inceleyelim :
//Öncelikle kütüphanenin sınıfı tanımlıyoruz
Gecis animasyon(1); // Buradaki 1 parametresi bir adet geçiş animasyonu kullanacağımızı belirtir. İstenilen kadar değer girilebilir
Gecis::Tween gecis1; // Animasyona ait olan özellikleri barındıracak olan Tween yapısı ile gecis1 adında değişken oluşturur

int pozisyon; // Değişkenin tanımlamasını yaptık
void setup() {
// Değişken adresi, ilk değer, son değer, toplam süre, tekrar et, terse dön, geçiş fonksiyonu
	gecis1 = {&a, 0, 1000, 1000, false, true, YUMUSAK_GIRIS};
  	animasyon.ekle(&gecis1); // Gecisi ekliyoruz
}
void loop() {
 animasyon.guncelle(); // Gecislerin çalışabilmesi için animasyonların güncellenmesi gerekir
 
 if(!t1.bitti){
//Geçiş sona erdi tekrar çalışabilmesi için tekrar eklenmesi gerekir
 }
}
*/
#include "Gecis.h"

Gecis::Gecis(int length){
	tween_length = length;
	tweens = (Tween**)calloc(tween_length, sizeof(Tween*));
}

bool Gecis::ekle(Tween* _tween){
	if(tween_index >= tween_length)
		return false;

	_tween->bas_zaman = millis();
	
	/*if((_tween->bas_zaman + _tween->toplam_zaman) <= _tween->toplam_zaman) //millis() taşar ise ekleme yapma
		return false;*/
	
	_tween->bitti = false;
	tweens[tween_index] = _tween; // tween adresini diziye ata
	tween_index++;
	
	if(_tween->fonk > 12 || _tween->fonk < 0)
		_tween->fonk = 0;
	
	return true;
}

bool Gecis::sil(Tween* _tween){
	if(tween_index <= 0)
		return false;
	
	char i;
	for(i=0; i<tween_index; i++){
		if(tweens[i] == _tween){ // dizideki adres ile tween adresi aynı ise
			tweens[i] = NULL;
			duzenle();
			return true;
		}
	}
	return false;
}

void Gecis::duzenle(){
	int gercek_index = 0;
	int i;
	for(i=0; i<tween_length; i++){ //Nesneleri boşluklara kaydır
		if(tweens[i] != NULL){
			if(gercek_index != i){
				tweens[gercek_index] = tweens[i];
				tweens[i] = NULL;
			}
			gercek_index++;
		}
	}
	tween_index = gercek_index;
}


bool Gecis::guncelle(){
	if(tween_index <= 0)
		return false;
		
	for(int i=0; i<tween_index; i++){
		if(tweens[i] != NULL){
			tweens[i]->periyot = false;
			gecen_zaman = millis() - tweens[i]->bas_zaman;
			if(gecen_zaman >= tweens[i]->toplam_zaman){
				gecen_zaman = tweens[i]->toplam_zaman;
				*(tweens[i]->degisken) = tweens[i]->son_deger;
				
				if(tweens[i]->ters){
					int temp = tweens[i]->ilk_deger;
					tweens[i]->ilk_deger = tweens[i]->son_deger;
					tweens[i]->son_deger = temp;
					tweens[i]->bas_zaman = millis();
					
					if(!tweens[i]->tekrar)
						tweens[i]->ters = false;
					else
						tweens[i]->periyot = true;
				}
				else if(tweens[i]->tekrar){
					tweens[i]->bas_zaman = millis();
					tweens[i]->periyot = true;
				}
				else{
					tweens[i]->bitti = true;
					sil(tweens[i]);
				}
			}
			else{
				this->gFonk = fonksiyonlar[tweens[i]->fonk];
				float oran = (float)gecen_zaman / (float)tweens[i]->toplam_zaman;
				*(tweens[i]->degisken) = tweens[i]->ilk_deger + round((tweens[i]->son_deger - tweens[i]->ilk_deger) * (this->*gFonk)(oran));
			}
		}
	}
return true;
}

int Gecis::index(){
	return tween_index;
}


float Gecis::dogrusal(float oran){
	return oran;
}

float Gecis::yumusakGiris(float oran){
  return oran * oran * oran;
}

float Gecis::yumusakCikis(float oran){
  float tersOran = oran - 1;
  return tersOran * tersOran * tersOran +1; 
}

float Gecis::yumusakGirisElastik(float oran){
	if(oran == 1 || oran ==0)
		return oran;
	else{
		float p = 0.3;
		float s = p/4.0;
		float tersOran = oran - 1;
		return -1.0 * pow(2.0, 10.0 * tersOran) * sin((tersOran-s)*(2.0* 3.14)/p);
	}
}

float Gecis::yumusakCikisSekme(float oran){
	float s= 7.5625;
	float p= 2.75;
	float l;
	if (oran < (1.0/p)){
		l = s * pow(oran, 2);
	}
	else{
		if (oran < (2.0/p)){
			oran -= 1.5/p;
			l = s * pow(oran, 2) + 0.75;
		}
		else{
			if (oran < 2.5/p){
				oran -= 2.25/p;
				l = s * pow(oran, 2) + 0.9375;
			}
			else{
				oran -= 2.625/p;
				l =  s * pow(oran, 2) + 0.984375;
			}
		}
	}
	return l;
}

float Gecis::yumusakGirisGeri(float oran){
	float s = 1.70158;
	return pow(oran, 2) * ((s + 1.0)*oran - s);
}
		
float Gecis::yumusakCikisGeri(float oran){
	float tersOran = oran - 1.0;            
	float s = 1.70158;
	return pow(tersOran, 2) * ((s + 1.0)*tersOran + s) + 1.0;
}

float Gecis::yumusakCikisElastik(float oran){
	if (oran == 0 || oran == 1) return oran;
	else{
		float p = 0.3;
		float s = p/4.0;                
		return pow(2.0, -10.0*oran) * sin((oran-s)*(2.0*3.14)/p) + 1;                
	}            
}      	

float Gecis::yumusakGirisCikis(float oran){
	return yumusakBirlesim(&Gecis::yumusakGiris, &Gecis::yumusakCikis, oran);
} 

float Gecis::yumusakCikisGiris(float oran){
	return yumusakBirlesim(&Gecis::yumusakCikis, &Gecis::yumusakGiris, oran);
} 

float Gecis::yumusakGirisSekme(float oran){
	return 1.0 - yumusakCikisSekme(1.0 - oran);
}

float Gecis::yumusakGirisCikisElastik(float oran){
	return yumusakBirlesim(&Gecis::yumusakGirisElastik, &Gecis::yumusakCikisElastik, oran);
}   

float Gecis::yumusakCikisGirisElastik(float oran){
	return yumusakBirlesim(&Gecis::yumusakCikisElastik, &Gecis::yumusakGirisElastik, oran);
}

float Gecis::yumusakBirlesim(gecisFonk baslangicFonk, gecisFonk bitisFonk, float oran){
	if (oran < 0.5) {
		return 0.5 * (this->*baslangicFonk)(oran * 2.0);
	}
	else {
		return 0.5 * (this->*bitisFonk)((oran - 0.5) * 2.0) + 0.5;
	}	
}







