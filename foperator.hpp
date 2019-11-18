#ifndef RCL_TEMPLATE_DEFAULT_OPERATORS_BEHAVIOUR
#define RCL_TEMPLATE_DEFAULT_OPERATORS_BEHAVIOUR

// Default operator behaviour

//template <typename T> inline bool operator==(const X& lhs, const X& rhs){ /* do actual comparison */ }

//template <typename T> inline bool operator< (const X& lhs, const X& rhs){ /* do actual comparison */ }

template <typename T> inline bool operator!=(const T& lhs, const T& rhs){return !operator==(lhs,rhs);}
template <typename T> inline bool operator> (const T& lhs, const T& rhs){return  operator< (rhs,lhs);}
template <typename T> inline bool operator<=(const T& lhs, const T& rhs){return !operator> (lhs,rhs);}
template <typename T> inline bool operator>=(const T& lhs, const T& rhs){return !operator< (lhs,rhs);}


#endif
