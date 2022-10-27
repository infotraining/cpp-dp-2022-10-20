
// MFCQuickEntryDlg.h : header file
//

#pragma once

#include <vector>
#include <string>
#include <functional>
#include <memory>

using ListModel = std::vector<std::string>;

struct IListView
{
    virtual void ClearEditLine() = 0;
    virtual void AddItemToList(const std::string& item) = 0;
    virtual void RemoveItemFromList(int index) = 0;
    virtual void SetListItems(const std::vector<std::string>& items) = 0;
    virtual void EnableBtnAdd(bool enabled) = 0;
    virtual void EnableBtnRemove(bool enabled) = 0;
    virtual void ClearSelectionOnList() = 0;
    virtual bool SelectItemOnList(const std::string& item) = 0;
    virtual int GetIndexOfSelectedItem() = 0;
    std::function<void(const std::string&)> ChangeTextEvent;
    std::function<void(const std::string&)> AddItemEvent;
    std::function<void()> RemoveItemEvent;
    virtual ~IListView() = default;
};

class ListPresenter
{
    ListModel& model_;
    IListView& view_;

public:
    ListPresenter(ListModel& model, IListView& view)
        : model_{ model }
        , view_{ view }
    {
        view_.ChangeTextEvent = [this](const std::string& txt) {
            OnTextChanged(txt);
        };

        view_.AddItemEvent = [this](const std::string& txt) {
            OnAddItem(txt);
        };

        view_.RemoveItemEvent = [this]() {
            OnRemoveItem();
        };
    }

    void Init()
    {
        view_.EnableBtnAdd(false);
        view_.EnableBtnRemove(false);
        view_.SetListItems(model_);
    }

    void OnTextChanged(const std::string& text)
    {
        if (view_.SelectItemOnList(text))
        {
            view_.EnableBtnAdd(false);
            view_.EnableBtnRemove(true);
        }
        else
        {
            view_.ClearSelectionOnList();
            view_.EnableBtnAdd(true);
            view_.EnableBtnRemove(false);
        }
    }

    void OnAddItem(const std::string& item)
    {
        model_.push_back(item);
        view_.EnableBtnAdd(false);
        view_.ClearEditLine();
        view_.AddItemToList(item);
    }

    void OnRemoveItem()
    {
        int index = view_.GetIndexOfSelectedItem();
        assert(index >= 0);
        model_.erase(model_.begin() + index);
        view_.EnableBtnRemove(false);
        view_.ClearEditLine();
        view_.RemoveItemFromList(index);
    }
};

// CMFCQuickEntryDlg dialog
class CMFCQuickEntryDlg : public CDialogEx, public IListView
{
// Construction
public:
	CMFCQuickEntryDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCQUICKENTRY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonAddClick();
	afx_msg void OnListChange();
	DECLARE_MESSAGE_MAP()
public:
	CEdit edtBand;
	CListBox lstBands;
	CButton btnAdd;
	CButton btnRemove;
	afx_msg void OnBnClickedButtonRemove();

    void ClearEditLine();
    void AddItemToList(const std::string& item);
    void RemoveItemFromList(const std::string& item);
    void SetListItems(const std::vector<std::string>& items);
    void EnableBtnAdd(bool enabled);
    void EnableBtnRemove(bool enabled);
    void ClearSelectionOnList();
    bool SelectItemOnList(const std::string& item);
    int GetIndexOfSelectedItem();
    void RemoveItemFromList(int index);
private:
    ListModel lstModel{ "Alice in Chains", "Metallica", "Pearl Jam", "Tool" };
    std::unique_ptr<ListPresenter> presenter;
};
