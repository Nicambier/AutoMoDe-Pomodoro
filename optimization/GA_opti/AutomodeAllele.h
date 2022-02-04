#include <typeinfo>
#include <string>
#include <cmath>
#include <type_traits>
#include <vector>

class AutomodeAllele {
public:
    AutomodeAllele(std::string name): m_name(name), size(0) {}
    AutomodeAllele(const AutomodeAllele& a): m_name(a.m_name), size(a.size) {}
    virtual AutomodeAllele& clone() = 0;
    virtual ~AutomodeAllele() {}
    
    int getLength() const {return size;}
    std::string getName() const {return m_name;}
    virtual double value(const GABit* data, int beginIdx, int lastId = 0) const = 0;
    
protected:    
    std::string m_name;
    int size;
};

class AutomodeBaseAllele: public AutomodeAllele {
public:
    AutomodeBaseAllele(std::string name, double min, double max, bool isFloat=false): AutomodeAllele(name), m_min(min), m_max(max), m_isFloat(isFloat) {size = std::ceil(std::log2(bit_range()));}
    AutomodeBaseAllele(const AutomodeBaseAllele& a): AutomodeAllele(a), m_min(a.m_min), m_max(a.m_max), m_isFloat(a.m_isFloat) {}
    virtual ~AutomodeBaseAllele() {}
    
    virtual AutomodeAllele& clone() {return *new AutomodeBaseAllele(*this);}
    
    double value(const GABit* data, int beginIdx, int lastId = 0) const {
        double val = 0;
        for(int i = beginIdx; i<beginIdx+getLength(); ++i)
            val += data? std::pow(2,i):0;
        return m_min + ((unsigned int) (val / type_factor()) % bit_range());
    }
    
private:
    unsigned int type_factor() const {return m_isFloat?100:1;}
    unsigned int bit_range() const {return (m_max - m_min) * type_factor() + 1;}
    
    double m_min, m_max;
    bool m_isFloat;
};

class AutomodeAltAllele: public AutomodeAllele {
public:
    AutomodeAltAllele(std::string name): AutomodeAllele(name) {}
    AutomodeAltAllele(const AutomodeAltAllele& a): AutomodeAllele(a) {for(std::vector<AutomodeBaseAllele*>::const_iterator iter=a.m_alts.begin();iter!=a.m_alts.end();++iter) push_back(new AutomodeBaseAllele(**iter));}
    virtual AutomodeAllele& clone() {return *new AutomodeAltAllele(*this);}
    ~AutomodeAltAllele() {for(unsigned int i=0; i<m_alts.size(); ++i) delete m_alts[i]; m_alts.clear();}
    unsigned int push_back(double min, double max, bool isFloat=false) {
        m_alts.push_back(new AutomodeBaseAllele(m_name,min,max,isFloat)); 
        size = longestAlt(); 
        return getLength();}
        
    unsigned int push_back(AutomodeBaseAllele* a) {
        m_alts.push_back(a); 
        size = longestAlt(); 
        return getLength();}
        
    double value(const GABit* data, int beginIdx, int lastId) const {
        return m_alts[lastId]->value(data,beginIdx);
    }
    
private:
    int longestAlt() {
        int size=0;
        for(long unsigned int i=0; i<m_alts.size(); ++i)
            if(m_alts[i]->getLength() > size)
                size = m_alts[i]->getLength();
        return size;
    }
    
    std::vector<AutomodeBaseAllele*> m_alts;
};
