# Sensor-Data-Simulation-and-Processing

## Kurulum ve Çalıştırma Talimatları

Kodunuzu derlemek için bir terminal açın ve şu komutu çalıştırın:

- **Derleme:** `g++ main.cpp -o main.exe`
- **Programı Çalıştırma:** `./main.exe`


## Parametre Değiştirme:

Program başlatıldıktan sonra, varsayılan parametrelerle çalıştırılır. Ancak, kullanıcıya y/n seçeneği ile parametreleri güncelleme imkanı sunulmaktadır.

## Varsayımlar

-Sensör verisinin aralığı: -dataRange ile +dataRange arasında rastgele üretilmiştir. Varsayılan dataRange değeri 100’dür, ancak kullanıcı tarafından değiştirilebilir.

-Varsayılan Zaman Periyodu: Veri üretimi periyodik modda varsayılan olarak 1 saniyede bir yapılmaktadır, ancak kullanıcı tarafından TimePeriod değeri ile 
değiştirilebilir.

-Window Size (Pencere Boyutu): MovingAverageFilter modunda varsayılan olarak windowSize değeri 3’tür. Bu değer, kullanıcı tarafından belirlenebilir.
Empty Data Set Durumu: Veri işleme veya analiz işlemleri sırasında veri seti boş ise, bir hata mesajı gösterilir. Ardından program sonlandırılır.

## Tasarım Seçimleri

Sensor Sınıfı:
Sensor sınıfı, veri üretme ve toplama görevini üstlenmiştir. Rastgele bir veri oluşturucusu kullanarak generateData() fonksiyonu ile veriler üretir.
collectData() fonksiyonu ile belirtilen sayıda veri toplayarak queue yapısında saklar. Böylece veri seti asenkron veya periyodik olarak toplanabilir.

DataProcessor Sınıfı:
DataProcessor, Sensor sınıfından bağımsız olarak, sensörden alınan veriyi işleyen bir sınıftır. SimpleAverageFilter ve MovingAverageFilter gibi filtreler ile veriyi işler. Veri analizi için getMin(), getMax(), ve getAverage() fonksiyonları ile minimum, maksimum ve ortalama değerleri hesaplar. Bu fonksiyonlar, boş veri seti durumunda uygun hata mesajları döndürür. Bonus olarak printFilteredData(), saveDatas() methodları ve ayrıca setParameters() ve printParameters() fonksiyonları kullanılmıştır. printFilteredData() ile ekrana yazdırılan değerler ayrıca saveDatas() ile output.txt dosyasına kaydedilir. printParameters() ile parametreler yazdırıllır. setParameters() fonksiyonu ile kullanıcıdan alınan tüm girdiler doğrulanır ve hatalı giriş durumunda uyarı mesajları gösterilir. setParameters() fonksiyonu ile kullanıcı, zaman periyodu, pencere boyutu, veri aralığı gibi parametreleri program başında değiştirebilir.


Exception Handling:
Boş veri seti gibi olası hata durumları ele alınmıştır. cerr ile hata mesajları verilmiş ve exit() kullanılmıştır. 
Kullanıcının hatalı giriş yapması durumunda, girişler doğrulanarak hata mesajları gösterilmiştir.
