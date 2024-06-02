#pragma once
#include "dt_common.h"
#include "dt_recv.h"

/**
 * Represents a redefined receive property.
 */
class RecvPropRedef
{
public:
    /**
     * Constructs a RecvPropRedef object.
     */
    RecvPropRedef();

    /**
     * Initializes the array properties of the object.
     * @param nElements The number of elements in the array.
     * @param elementStride The stride of each element in the array.
     * @throws std::runtime_error If the object has already been initialized.
     */
    void InitArray(int nElements, int elementStride);

    /**
     * Gets the number of elements in the array.
     * @return The number of elements in the array.
     */
    int GetNumElements() const;

    /**
     * Sets the number of elements in the array.
     * @param nElements The number of elements in the array.
     */
    void SetNumElements(int nElements);

    /**
     * Gets the stride of each element in the array.
     * @return The stride of each element in the array.
     */
    int GetElementStride() const;

    /**
     * Sets the stride of each element in the array.
     * @param stride The stride of each element in the array.
     */
    void SetElementStride(int stride);

    /**
     * Gets the flags of the property.
     * @return The flags of the property.
     */
    int GetFlags() const;

    /**
     * Gets the name of the property.
     * @return The name of the property.
     */
    const char* GetName() const;

    /**
     * Gets the type of the property.
     * @return The type of the property.
     */
    SendPropType GetType() const;

    /**
     * Gets the data table of the property.
     * @return The data table of the property.
     */
    RecvTable* GetDataTable() const;

    /**
     * Sets the data table of the property.
     * @param pTable The data table of the property.
     */
    void SetDataTable(RecvTable* pTable);

    /**
     * Gets the proxy function of the property.
     * @return The proxy function of the property.
     */
    RecvVarProxyFn GetProxyFn() const;

    /**
     * Sets the proxy function of the property.
     * @param fn The proxy function of the property.
     */
    void SetProxyFn(RecvVarProxyFn fn);

    /**
     * Gets the data table proxy function of the property.
     * @return The data table proxy function of the property.
     */
    DataTableRecvVarProxyFn GetDataTableProxyFn() const;

    /**
     * Sets the data table proxy function of the property.
     * @param fn The data table proxy function of the property.
     */
    void SetDataTableProxyFn(DataTableRecvVarProxyFn fn);

    /**
     * Gets the offset of the property.
     * @return The offset of the property.
     */
    int GetOffset() const;

    /**
     * Sets the offset of the property.
     * @param o The offset of the property.
     */
    void SetOffset(int o);

    /**
     * Gets the array property of the object.
     * @return The array property of the object.
     */
    RecvProp* GetArrayProp() const;

    /**
     * Sets the array property of the object.
     * @param pProp The array property of the object.
     */
    void SetArrayProp(RecvProp* pProp);

    /**
     * Sets the array length proxy of the object.
     * @param proxy The array length proxy of the object.
     */
    void SetArrayLengthProxy(ArrayLengthRecvProxyFn proxy);

    /**
     * Gets the array length proxy of the object.
     * @return The array length proxy of the object.
     */
    ArrayLengthRecvProxyFn GetArrayLengthProxy() const;

    /**
     * Checks if the property is inside an array.
     * @return True if the property is inside an array, false otherwise.
     */
    bool IsInsideArray() const;

    /**
     * Sets the flag indicating that the property is inside an array.
     */
    void SetInsideArray();

    /**
     * Gets the extra data of the property.
     * @return The extra data of the property.
     */
    const void* GetExtraData() const;

    /**
     * Sets the extra data of the property.
     * @param pData The extra data of the property.
     */
    void SetExtraData(const void* pData);

    /**
     * Gets the parent array property name of the property.
     * @return The parent array property name of the property.
     */
    const char* GetParentArrayPropName();

    /**
     * Sets the parent array property name of the property.
     * @param pArrayPropName The parent array property name of the property.
     */
    void SetParentArrayPropName(const char* pArrayPropName);

public:
    const char* m_pVarName; /**< The name of the property. */
    SendPropType m_RecvType; /**< The type of the property. */
    int m_Flags; /**< The flags of the property. */
    int m_StringBufferSize; /**< The buffer size of the string property. */

private:
    bool m_bInsideArray; /**< The flag indicating that the property is inside an array. */
    const void* m_pExtraData; /**< The extra data of the property. */
    RecvProp* m_pArrayProp; /**< The array property of the object. */
    ArrayLengthRecvProxyFn m_ArrayLengthProxy; /**< The array length proxy of the object. */
    RecvVarProxyFn m_ProxyFn; /**< The proxy function of the property. */
    DataTableRecvVarProxyFn m_DataTableProxyFn; /**< The data table proxy function of the property. */
    RecvTable* m_pDataTable; /**< The data table of the property. */
    int m_Offset; /**< The offset of the property. */
    int m_ElementStride; /**< The stride of each element in the array. */
    int m_nElements; /**< The number of elements in the array. */
    const char* m_pParentArrayPropName; /**< The parent array property name of the property. */
};
