#pragma once

#include "block_view.h"

#include "WndDesign/figure/text_block.h"
#include "WndDesign/common/unicode_helper.h"


class PairView;


class TextView : public BlockView, public LayoutType<Assigned, Auto> {
public:
	TextView(BlockView& parent, std::wstring text);

	// parent
private:
	PairView& GetParent();

	// data
private:
	virtual void Load() override;
	virtual void Save() override;

	// text
private:
	using HitTestInfo = TextBlock::HitTestInfo;
private:
	std::wstring text;
	TextBlock text_block;
	WordBreakIterator word_break_iterator;
private:
	size_t GetCharacterLength(size_t text_position) { return GetUTF16CharLength(text[text_position]); }
	void TextUpdated();

	// layout
private:
	Size size;
private:
	Size UpdateLayout();
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override;

	// paint
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;

	// caret
private:
	void RedrawCaretRegion();
private:
	void SetCaret(const HitTestInfo& info);
public:
	void SetCaret(size_t text_position);
private:
	virtual void SetCaret(Point point) override;
	virtual void ClearCaret() override;

	// selection
private:
	void RedrawSelectionRegion();
	void UpdateSelectionRegion(size_t begin, size_t count);
private:
	void SelectWord();
private:
	virtual bool HitTestSelection(Point point) override;
	virtual void BeginSelect(BlockView& child) override;
	virtual void DoSelect(Point point) override;
	virtual void SelectMore() override;
	virtual void ClearSelection() override;

	// drag and drop
private:
	void RedrawDragDropCaretRegion();
private:
	virtual void DoDragDrop(BlockView& source, Point point) override;
	virtual void CancelDragDrop() override;

	// modify
private:
	void TextModified() { DataModified(); TextUpdated(); }
private:
	void InsertText(size_t pos, wchar ch) { text.insert(pos, 1, ch); TextModified(); }
	void InsertText(size_t pos, std::wstring str) { text.insert(pos, str); TextModified(); }
	void ReplaceText(size_t begin, size_t length, wchar ch) { text.replace(begin, length, 1, ch); TextModified(); }
	void ReplaceText(size_t begin, size_t length, std::wstring str) { text.replace(begin, length, str); TextModified(); }
	void DeleteText(size_t begin, size_t length) { text.erase(begin, length); TextModified(); }

	// route
public:
	TextView& MergeBackWith(TextView& text_view);  // text backspace, text delete

	// input
private:
	virtual void FinishDragDrop(BlockView& source) override;
private:
	void Insert(wchar ch);
	void Insert(std::wstring str);
	void Split();
	void Indent();
	void Delete(bool is_backspace);
private:
	void Cut();
	void Copy();
	void Paste();
private:
	void OnImeBegin();
	void OnImeString();
	void OnImeEnd();

	// message
private:
	virtual void OnKeyMsg(KeyMsg msg) override;
};
