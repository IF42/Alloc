# Alloc

# Paměťový model procesu
Paměťový model procesu popisuje, jak je paměť organizována a jak s ní procesy pracují. Obecně se skládá z několika hlavních oblastí, které určují, jak se přistupuje k datům a instrukcím během běhu programu. 
Proces běžící na moderních systémech má paměť rozdělenou oblasti:
* Text - část paměti kam se ukládají instrukce a konstanty programu 
* Data - část paměti kde jsou uložené globální a statické inicializované proměnné
* Bss - část paměti kde jsou uloženy globální a statické neinicializované proměnné (jsou nastaveny na počáteční hodnotu 0)
* Heap - část paměti, která slouží k dynamickému přidělování za běhu programu
* Stack - část paměti, která slouží k ukládání kontextu volané funkce (lokální proměnné, návratové hodnoty, ukazatel na zásobník) 

# Virtuální paměť procesu
Virtuální paměť je mechanismus správy paměti, který umožňuje každému procesu pracovat s vlastním izolovaným adresním prostorem, aniž by přímo manipuloval s fyzickou pamětí RAM. Operační systém (OS) pomocí Memory Management Unit (MMU) překládá virtuální adresy na fyzické adresy. Díky tomu je paměť každého jednotlivých procesů spuštěných na jednom PC izolovaná a vzájemně se nemohou narušovat. Zároveň z pohledu běžícího procesu se jeví, že má k dispozici celou paměť RAM pouze pro sebe. Důležité ale je, že procesu je tato paměť přidělována postupně tak jak je potřeba podle toho kolik jí daný proces potřebuje.

# Fragmetnace paměti
Fragmentace paměti nastává, když je celkově dost volné paměti, ale není spojitá, takže ji nelze efektivně využít. To znamená, že data uložená v paměti na sebe nepřiléhají a mají mezi sebou nevyužitá místa, což může způsobit neefektivní využití paměti a snížení výkonu systému. Existují dva hlavní typy fragmentace: Externí a interní


## Coalescing 
Coalescing (slučování, spojování) je technika používaná ve správě paměti ke snížení fragmentace tím, že spojuje sousední volné bloky do jednoho většího bloku.

# Dynamická alokace (přidělovaní) paměti
V některých situacích není možné předem přesně říct kolik paměti bude daný algoritmus vyžadovat, proto je nutné si buď rezervovat při překladu dostečné množství paměti, což ale může způsobit plýtvání paměti a nebo dané množtví naopak nemusí stačit a nebo je možné si za běhu říct o přidělení přeného množství paměti, které je zrovna potřeba. Při dynamické alokaci se přiděluje nová paměť běžícímu procesu z oblasti paměti Heap. Díky dynamciké alokaci se neplývá pamětí a je možné ji po tom co již není potřeba vrátit operačnímu systému, který ji může opět přidělit jinému procesu.

## Životnost paměti
Životnost paměti říká, jak dlouho bude daná paměť použita v kontextu vykonávání programu. 
* Permanentně alokovaná paměť - paměť, která je využívána po celou dobu běhu programu
* Přechodně alokovaná paměť - paměť která je využívána pouze v rámci určitého cyklu nebo v určitém úseku vykonávání programu. Poté co je daný úsek kódu opuštěn je tato paměť uvolněna. 
* Dočasná paměť - paměť která je alokována pouze jako dočasné uložiště pro uložení například mezivýsledků a je uvolněna okamžitě jakmile již nejsou dat v této paměti potřeba.

## Automatická správa paměti
Automatická správa paměti je mechanismus, který se stará o alokaci a uvolňování paměti bez přímého zásahu programátora. Cílem je zabránit únikům paměti a dereferencím neplatných ukazatelů. Existuje několik přístupů k automatické správě paměti:

* Garbage Collection - Jedná se o nejběžnější formu automatické správy paměti kdy na pozadí běžícího programu běží algoritmus garbage collectoru a detekuje objekty, které už nejsou dosažitelné, a následně je odstraní. Jedná se o bezpečný způsob správy paměti, ale zároveň garbage collector musí běží asynchronně k danému procesu a může způsobit nečekané pauzy (stop-the-world efekt). To vyžaduje nějaký výkon procesoru a zároveň to zvyšuje spotřebu paměti kvůli metadatům na sledování paměti. 
* Reference Counting - každá alokovaná instance paměti si uchovává počet referencí v programu, která se průběžně aktualizuje a jakmile počet referencí klesne na nulu, je jisté, že na danou paměť již nemá žádná část programu přístup a je možné ji z paměti uvolnit. Výhodou je že zde nejsou potřeba stop-the-world GC pauzy a díky tomu je tento algoritmus použitelný i na systémech s potřebou nízké latence. Pro správné fungování je ale potřeba podpora jazyka. 
* Manuální správa paměti - Programátor má k dispozici funkce, kterými si může sám přidělit potřebné množství paměti a poté co ji už nepotřebuje ji může opět uvolnit. Tento proces je velice rychlý a umožňuje nízkou latenci programu, ale může být zdrojem chyb (dangling pointer, memory leak, double free, ...)
* Stack-based alokace - alokace založená na přidělování paměti na zásobníku, je rychlá a bezpečná, ale ne vždy je dostačující. Programátor musí vždy vědět kolik paměti aktuálně potřebuje, aby ji překladač mohl při volání funkce rezervovat. Při opuštění funkce je přidělená paměť automaticky uvolněna, nehrozí tak problém úniku paměti. 
* Paměťové alokátory - paměťové alokátry umožňují trackovat přidělenou paměť a ve chvíli kdy již dané přidělné paměťové instance nejsou potřeba je možné je všechny uvolnit najednou. To může za určitých okolností ne jen zrychlit proces alokace, ale zároveň zjednodušit správu většího počtu paměťových instancí protože není potřeba řešit nějaký složitý mechanismus správy paměti. Tento způsob správy paměti ale stále vyžaduje stejně jako v případě manuální správy paměti explicitně říct kdy se má paměť uvolnit.

# Rozrhaní
V programování rozhraní (interface) definuje způsob, jakým mohou různé komponenty systému komunikovat. V případě, že se definuje rozhraní, které daná struktura implementuje, je možné abstrahovat konkrétní inforamce o tom, jak je daná komponenta na nízké úrovni implementována. Díky tomu je možné používat různé komponenty se stejným rozhraním jednotným způsobem a není potřeba se starat o to, jak uvnitř funguje, ale poze o to, že dělá to co je třeba.
V případě, že se vytvoří obecné rozhraní pro alokátory, je možné funkcím, které vyžadují dynamickou alokaci předávat instanci rozhraní Alloc, ale nemusejí se zajímat o to, s jakým typem alokátoru pracují. 
Proto všechny allokátory mají implementované rozhraní Alloc, které obsahují virtuální funkce pro ovládání dynamické alokace.

# Zarovnání paměti
Moderní procesory přistupují k paměťovým adresám blokově, to znamená, že načítají do vnitřních cache pamětí určitý úsek RAM paměti najednou a následně se se určuje které části daného bloku se použijí pro aritmeticko-logické operace. Tento blok má vždy adresu zarovnanou na určitý násobek, který většinou odpovídá bitové šířce dané architektury procesoru a do cache pamětí procesoru je načteno více těchto bloků najednou. I když je RAM paměť mnohonásobně rychlejší na čtení nebo zápis než pevný disk, je stále pomalejší než procesor. Pokud program obsahuje instrukce, které několikrát po sobě pracují s daty, které jsou uloženy v paměti blízko u sebe (například pole, nebo struktury), je výhodně když je obsah těchto struktur, načten do rychlých cache pamětí procesoru, na které může velice rychle přistupovat a není bržděn pomalejší RAM pamětí. Aby se zajistilo, že jsou proměnné v RAM paměti umístěné tak, aby se při načtení určitého bloku nestalo, že je načtena pouze část, je nutné zajistit správné mapování. Tomu se říká zarovnání paměti (Memory Aligment). K tomu je zapotřebí, aby proměnné v strukturách byly místěny na paměťových adresách pole svého datového typu. V případě, že by došlo k uložení na špatnou pozici, vloží kompilátor mezi dvě proměnné tzv. "padding", extra paměť, která se nevyužívá pro uložení hodnot, ale zajistí, že se následující proměnná bude nacházet na vhodné adrese. Zároveň je třeba vhodně rozmístit do paměti globální a lokální proměnné, ale kompilátor se snaží je rozmístit tak, aby nebylo plýtváno s pamětí a nemusel vkládata padding mezi jednotlivé proměnné. 
## Cache hit a cache miss
V případě, že procesor požaduje data, která jsou načtena v cache paměti, jedná se o situaci, která se nazývá chace hit (data jsou poskytnuta z cache velmi rychle). Naopak cache miss je situace, kdy procesor potřebuje načíst data z RAM paměti, protože se nenacházejí v paměti cache. Cílem je orzanizovat data v RAM paměti tak, aby se minimalizovaly situace, kdy procesor musí do cache paměti načíst data, která v cache paměti nejsou. Tento stav může nastat z různích důvodů, ale vždy je to problém, protože procesoru, musí čekat než budou data v cache paměti dostupná. To zpomaluje vykonávaný program. 


# Arena Allocator
Arena nebo také Linear Allocator je jeden z nejjednodušších strategií alokátorů. Jak jméno napovídá, paměť je alokována lineárně z nějakého velkého bloku. Principem Arena alokátoru je offset, který udává začátek volného bloku paměti, který se posouvá tak jak je daná paměť postupně alokovaná. Díky tomu je alokace tak jednoduchá jako inkrementace hodnoty offsetu a to činí alokaci velice rychlou. Nevýhodou je, že Arena allokátor neumožňuje uvolňovat specifické oblasti alokované paměti a buď umožňuje zásobníkové uvolňování, tedy lze uvolňovat paměť pouze v opačném pořadí než byla alokována. Arena alokátor se běžněji uvolňuje celý najednou a je používán jako kontext výpočtu uvnitř nějaké funkce.


# Free list allocator


# Zdroje
* Understanding and using C pointers - Richard Reese
* Computer Systems: A Programmer's Perspective - Randal E. Bryant, David R. O'Hallaron
* [Designing custom memory allocator: malloc](https://medium.com/howsofcoding/custom-memory-allocator-malloc-62d28e10bfb8)
* [Designing custom memory allocator: free](https://medium.com/howsofcoding/memory-allocator-design-free-17db024aa030)
* [Designing custom memory allocator: aligned malloc and free](https://medium.com/howsofcoding/memory-management-aligned-malloc-and-free-9273336bd4c6)
* [memory allocation strategy](https://www.gingerbill.org/series/memory-allocation-strategies/)
* [malloc and free implementation](https://medium.com/@adambrandizzi/implementing-malloc-and-free-adding-metadata-to-the-memory-blocks-ecf8573b6f58)


