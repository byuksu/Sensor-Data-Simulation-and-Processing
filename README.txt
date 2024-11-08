Kurulum ve Çalıştırma Talimatları

Kodunuzu derlemek için bir terminal açın ve şu komutu çalıştırın:

Derleme: g++ main.cpp -o SensorDataApp
Programı Çalıştırma: ./SensorDataApp

Parametre Değiştirme:

Program başlatıldıktan sonra, varsayılan parametrelerle çalıştırılır. Ancak, kullanıcıya y/n seçeneği ile parametreleri güncelleme imkanı sunulmaktadır.

Geliştirme Sürecindeki Varsayımlar

-Sensör verisinin aralığı: -dataRange ile +dataRange arasında rastgele üretilmiştir. Varsayılan dataRange değeri 100’dür, ancak kullanıcı tarafından değiştirilebilir.

-Varsayılan Zaman Periyodu: Veri üretimi periyodik modda varsayılan olarak 1 saniyede bir yapılmaktadır, ancak kullanıcı tarafından TimePeriod değeri ile 
değiştirilebilir.

-Window Size (Pencere Boyutu): MovingAverageFilter modunda varsayılan olarak windowSize değeri 3’tür. Bu değer, kullanıcı tarafından belirlenebilir.
Empty Data Set Durumu: Veri işleme veya analiz işlemleri sırasında veri seti boş ise, bir hata mesajı gösterilir. Ardından program sonlandırılır.

Tasarım Seçimleri

Sensor Sınıfı:
Sensor sınıfı, veri üretme ve toplama görevini üstlenmiştir. Rastgele bir veri oluşturucusu kullanarak generateData() fonksiyonu ile veriler üretir.
collectData() fonksiyonu ile belirtilen sayıda veri toplayarak queue yapısında saklar. Böylece veri seti asenkron veya periyodik olarak toplanabilir.

DataProcessor Sınıfı:
DataProcessor, Sensor sınıfından bağımsız olarak, sensörden alınan veriyi işleyen bir sınıftır. SimpleAverageFilter ve MovingAverageFilter gibi filtreler ile veriyi işler. Veri analizi için getMin(), getMax(), ve getAverage() fonksiyonları ile minimum, maksimum ve ortalama değerleri hesaplar. Bu fonksiyonlar, boş veri seti durumunda uygun hata mesajları döndürür. Bonus olarak printFilteredData(), saveDatas() methodları ve ayrıca setParameters() ve printParameters() fonksiyonları kullanılmıştır. printFilteredData() ile ekrana yazdırılan değerler ayrıca saveDatas() ile output.txt dosyasına kaydedilir. printParameters() ile parametreler yazdırıllır. setParameters() fonksiyonu ile kullanıcıdan alınan tüm girdiler doğrulanır ve hatalı giriş durumunda uyarı mesajları gösterilir. setParameters() fonksiyonu ile kullanıcı, zaman periyodu, pencere boyutu, veri aralığı gibi parametreleri program başında değiştirebilir.


Exception Handling:
Boş veri seti gibi olası hata durumları ele alınmıştır. cerr ile hata mesajları verilmiş ve exit() kullanılmıştır. 
Kullanıcının hatalı giriş yapması durumunda, girişler doğrulanarak hata mesajları gösterilmiştir.

Çıktı Örneği:

Default parameters: 
--------------------------------------------------
windowSize = 3
TimePeriod = 1000 ms
Numpoints = 10
DataRange = 100
data_generation_timing = periodically
filterType = MovingAverageFilter
--------------------------------------------------
Do you want to change parameters? (y/n): y
Enter TimePeriod (positive integer in ms) less than 2000 (2s): 500
Enter Numpoints (positive integer) less than 200: 30
Enter windowSize (positive integer), winndowsize should be less than Numpoints (30) : 5
Enter dataRange (positive integer) ----> (-dataRange,dataRange) : 1000
Enter data_generation_timing (0 for periodically, 1 for asynchronously): 1
Enter filterType (0 for SimpleAverageFilter, 1 for MovingAverageFilter): 1
Updated parameters:
--------------------------------------------------
windowSize = 5
TimePeriod = 500 ms
Numpoints = 30
DataRange = 1000
data_generation_timing = asynchronously
filterType = MovingAverageFilter
--------------------------------------------------
Data received from the sensor: -515.5
Data received from the sensor: 773
Data received from the sensor: 256.2
Data received from the sensor: -988.4
Data received from the sensor: 739.2
Data received from the sensor: -679.1
Data received from the sensor: 895.8
Data received from the sensor: 72.9
Data received from the sensor: 336.9
Data received from the sensor: 862.8
Data received from the sensor: -847.3
Data received from the sensor: 684.2
Data received from the sensor: -149.8
Data received from the sensor: 349.6
Data received from the sensor: 456
Data received from the sensor: 82.8
Data received from the sensor: -898.3
Data received from the sensor: -876.3
Data received from the sensor: -979.3
Data received from the sensor: 418.7
Data received from the sensor: -570.1
Data received from the sensor: 15.3
Data received from the sensor: 671.3
Data received from the sensor: 408.4
Data received from the sensor: -345.2
Data received from the sensor: -858.3
Data received from the sensor: 497
Data received from the sensor: 712.4
Data received from the sensor: 644.9
Data received from the sensor: 363.5
=========================================
Number of Data Points  :30
Minimum Value          :-581.06
Maximum Value          :297.86
Average Value          :19.8077
=========================================