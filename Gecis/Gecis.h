#ifndef Gecis_h
#define Gecis_h
#include "arduino.h"

#define DOGRUSAL 0
#define YUMUSAK_GIRIS 1
#define YUMUSAK_CIKIS 2
#define YUMUSAK_GIRIS_SEKME 3
#define YUMUSAK_CIKIS_SEKME 4
#define YUMUSAK_GIRIS_GERI 5
#define YUMUSAK_CIKIS_GERI 6
#define YUMUSAK_GIRIS_ELASTIK 7 
#define YUMUSAK_CIKIS_ELASTIK 8
#define YUMUSAK_GIRIS_CIKIS 9
#define YUMUSAK_CIKIS_GIRIS 10
#define YUMUSAK_CIKIS_GIRIS_ELASTIK 11 
#define YUMUSAK_GIRIS_CIKIS_ELASTIK 12


class Gecis{
	public:
		Gecis(int);
		struct Tween{
			int* degisken;
			int ilk_deger;
			int son_deger;
			unsigned long toplam_zaman;
			bool tekrar;
			bool ters;
			char fonk;
			bool bitti;
			unsigned long bas_zaman;
			bool periyot;
		};
		
		/*	Tweens dizisindeki indeksin bilgisini tutar	*/
		int index();
		
		/*	Tweens dizisine, indeksin gösterdiği adrese tween ekler	*/
		bool ekle(Tween*);
		
		/*	Tweens dizisinden tween'i siler	*/
		bool sil(Tween*);
		
		/*	Tweens dizisindeki tweenleri zamana göre günceller*/
		bool guncelle();
		
	private:
		/* 	Tween yapısının göstergesine gösterge. Her bir gösterge 4byte
			Tweenlerin adreslerini tutan tek boyutlu dizi	*/
		Tween** tweens; 
		
		/*	Maksimum kaç tane tween göstergesinin olacağını tutan değişken */
		int tween_length = 0;
		
		/*	Tweens dizisindeki tweenlerin son indeksini gösterir, 
			tweenler silindikçe	bu değer azalır eklendikçe artar	*/
		int tween_index = 0;
		
		/*	Tween çalışmaya başladığı andan itibaren geçen süreyi 
			hesaplamak için kullanılan geçici bşr değişken	*/
		unsigned long gecen_zaman = 0;
		
		/*	Tweens dizisindeki tweenlerden çalışması bitenlerin veya silinenlerin 
			(NULL göstergelerin) oluşturduğu boşlukları kaldırır yeni tweenler için yer açar */
		void duzenle();
		
		/*	float parametreli ve float döndüren geçiş fonksiyonları için gösterge tanımlaması*/
		typedef float (Gecis::*gecisFonk)(float);
		gecisFonk gFonk;
		
		float yumusakBirlesim(gecisFonk, gecisFonk, float);
		float yumusakCikisGirisElastik(float);
		float yumusakGirisCikisElastik(float);
		float yumusakGirisSekme(float);
		float yumusakCikisGiris(float);
		float yumusakGirisCikis(float);
		float yumusakCikisElastik(float oran);
		float yumusakCikisGeri(float);
		float yumusakGirisGeri(float);
		float yumusakGiris(float);
		float yumusakCikis(float);
		float yumusakGirisElastik(float);
		float yumusakCikisSekme(float);
		float dogrusal(float);
	
		/*Geçiş fonksiyonlarının adreslerini tutan dizi*/
		const gecisFonk fonksiyonlar[13] = {
			&Gecis::dogrusal
			,&Gecis::yumusakGiris
			,&Gecis::yumusakCikis
			,&Gecis::yumusakGirisSekme
			,&Gecis::yumusakCikisSekme
			,&Gecis::yumusakGirisGeri
			,&Gecis::yumusakCikisGeri
			,&Gecis::yumusakGirisElastik
			,&Gecis::yumusakCikisElastik
			,&Gecis::yumusakGirisCikis
			,&Gecis::yumusakCikisGiris
			,&Gecis::yumusakGirisCikisElastik
			,&Gecis::yumusakCikisGirisElastik
		};
};

#endif