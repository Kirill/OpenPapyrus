// CmdROrderPrereqActivity.java
// Copyright (c) A.Sobolev 2021, 2022
//
package ru.petroglif.styloq;

import android.content.Context;
import android.content.Intent;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import androidx.annotation.IdRes;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.viewpager2.widget.ViewPager2;
import com.google.android.material.tabs.TabLayout;
import com.google.android.material.textfield.TextInputEditText;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class CmdROrderPrereqActivity extends SLib.SlActivity {
	public  CommonPrereqModule CPM;
	private JSONArray UomListData;
	private JSONArray WharehouseListData;
	private JSONArray QuotKindListData;
	private ArrayList <JSONObject> BrandListData;
	private ArrayList <CliEntry> CliListData;
	private ViewDescriptionList VdlDocs; // Описание таблицы просмотра существующих заказов

	public CmdROrderPrereqActivity()
	{
		CPM = new CommonPrereqModule(this);
	}
	private static class CliEntry {
		CliEntry(JSONObject jsItem)
		{
			JsItem = jsItem;
			AddrExpandStatus = 0;
			if(JsItem != null) {
				JSONArray dlvr_loc_list = JsItem.optJSONArray("dlvrloc_list");
				if(dlvr_loc_list != null && dlvr_loc_list.length() > 0)
					AddrExpandStatus = 1;
			}
		}
		public ArrayList <JSONObject> GetDlvrLocListAsArray()
		{
			ArrayList <JSONObject> result = null;
			JSONArray dlvr_loc_list = JsItem.optJSONArray("dlvrloc_list");
			if(dlvr_loc_list != null && dlvr_loc_list.length() > 0) {
				result = new ArrayList<JSONObject>();
				try {
					for(int i = 0; i < dlvr_loc_list.length(); i++) {
						Object dlvr_loc_list_item_obj = dlvr_loc_list.get(i);
						if(dlvr_loc_list_item_obj != null && dlvr_loc_list_item_obj instanceof JSONObject)
							result.add((JSONObject)dlvr_loc_list_item_obj);
					}
				} catch(JSONException exn) {
					result = null;
				}
			}
			return result;
		}
		int   AddrExpandStatus; // 0 - no addressed, 1 - addresses collapsed, 2 - addresses expanded
		JSONObject JsItem;
	}
	private void MakeSimpleSearchIndex()
	{
		if(CPM.SimpleSearchIndex == null)
			CPM.SimpleSearchIndex = new ArrayList<CommonPrereqModule.SimpleSearchIndexEntry>();
		else
			CPM.SimpleSearchIndex.clear();
		if(CPM.GoodsListData != null) {
			for(int i = 0; i < CPM.GoodsListData.size(); i++) {
				CommonPrereqModule.WareEntry ware_item = CPM.GoodsListData.get(i);
				if(ware_item != null && ware_item.JsItem != null) {
					int id = ware_item.JsItem.optInt("id", 0);
					if(id > 0) {
						String nm = ware_item.JsItem.optString("nm");
						if(SLib.GetLen(nm) > 0) {
							CPM.AddSimpleIndexEntry(SLib.PPOBJ_GOODS, id, SLib.PPOBJATTR_NAME, nm, null);
						}
						{
							JSONArray js_code_list = ware_item.JsItem.optJSONArray("code_list");
							if(js_code_list != null && js_code_list.length() > 0) {
								for(int j = 0; j < js_code_list.length(); j++) {
									JSONObject js_code = js_code_list.optJSONObject(j);
									if(js_code != null) {
										String code = js_code.optString("cod");
										if(SLib.GetLen(code) > 0)
											CPM.AddSimpleIndexEntry(SLib.PPOBJ_GOODS, id, SLib.PPOBJATTR_CODE, code, nm);
									}
								}
							}
						}
					}
				}
			}
		}
		if(BrandListData != null) {
			for(int i = 0; i < BrandListData.size(); i++) {
				JSONObject js_item = BrandListData.get(i);
				if(js_item != null) {
					int id = js_item.optInt("id", 0);
					if(id > 0) {
						String nm = js_item.optString("nm");
						if(SLib.GetLen(nm) > 0)
							CPM.AddSimpleIndexEntry(SLib.PPOBJ_BRAND, id, SLib.PPOBJATTR_NAME, nm, null);
					}
				}
			}
		}
		if(CPM.GoodsGroupListData != null) {
			for(int i = 0; i < CPM.GoodsGroupListData.size(); i++) {
				JSONObject js_item = CPM.GoodsGroupListData.get(i);
				if(js_item != null) {
					final int id = js_item.optInt("id", 0);
					if(id > 0) {
						String nm = js_item.optString("nm");
						if(SLib.GetLen(nm) > 0)
							CPM.AddSimpleIndexEntry(SLib.PPOBJ_GOODSGROUP, id, SLib.PPOBJATTR_NAME, nm, null);
					}
				}
			}
		}
		if(CliListData != null) {
			for(int i = 0; i < CliListData.size(); i++) {
				CliEntry ce = CliListData.get(i);
				if(ce != null && ce.JsItem != null) {
					int id = ce.JsItem.optInt("id", 0);
					if(id > 0) {
						String nm = ce.JsItem.optString("nm", null);
						if(SLib.GetLen(nm) > 0) {
							CPM.AddSimpleIndexEntry(SLib.PPOBJ_PERSON, id, SLib.PPOBJATTR_NAME, nm, nm);
						}
						String ruinn = ce.JsItem.optString("ruinn");
						if(SLib.GetLen(ruinn) > 0) {
							CPM.AddSimpleIndexEntry(SLib.PPOBJ_PERSON, id, SLib.PPOBJATTR_RUINN, ruinn, nm);
						}
						String rukpp = ce.JsItem.optString("rukpp");
						if(SLib.GetLen(rukpp) > 0) {
							CPM.AddSimpleIndexEntry(SLib.PPOBJ_PERSON, id, SLib.PPOBJATTR_RUKPP, rukpp, nm);
						}
						JSONArray dlvr_loc_list = ce.JsItem.optJSONArray("dlvrloc_list");
						if(dlvr_loc_list != null) {
							for(int j = 0; j < dlvr_loc_list.length(); j++) {
								JSONObject js_item = dlvr_loc_list.optJSONObject(j);
								if(js_item != null) {
									final int loc_id = js_item.optInt("id", 0);
									if(loc_id > 0) {
										String addr = js_item.optString("addr");
										if(SLib.GetLen(addr) > 0)
											CPM.AddSimpleIndexEntry(SLib.PPOBJ_LOCATION, loc_id, SLib.PPOBJATTR_RAWADDR, addr, nm);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	static class TransferItemDialog extends SLib.SlDialog {
		CmdROrderPrereqActivity ActivityCtx;
		public TransferItemDialog(Context ctx, Object data)
		{
			super(ctx, R.id.DLG_ORDRTI, data);
			if(ctx instanceof CmdROrderPrereqActivity)
				ActivityCtx = (CmdROrderPrereqActivity)ctx;
			if(data instanceof Document.TransferItem)
				Data = data;
		}
		private void StepQuantity(int direction) // direction < 0 - decrement; > 0 - increment
		{
			boolean do_update = false;
			if(Data != null && Data instanceof Document.TransferItem) {
				//Context ctx = getContext();
				//StyloQApp app_ctx = (StyloQApp)ctx.getApplicationContext();
				Document.TransferItem _data = (Document.TransferItem)Data;
				CommonPrereqModule.WareEntry goods_item = ActivityCtx.CPM.FindGoodsItemByGoodsID(_data.GoodsID);
				double _upp = 0.0;
				double _mult = 0.0;
				double _min = 0.0;
				if(goods_item != null && goods_item.JsItem != null) {
					_upp = goods_item.JsItem.optDouble("upp", 0.0);
					_mult = goods_item.JsItem.optDouble("ordqtymult", 0.0);
					_min = goods_item.JsItem.optDouble("ordminqty", 0.0);
				}
				String qtty_text = SLib.GetCtrlString(this, R.id.CTL_ORDRTI_QTTY);
				if(_data.Set == null)
					_data.Set = new Document.ValuSet();
				if(SLib.GetLen(qtty_text) > 0)
					_data.Set.Qtty = SLib.strtodouble(qtty_text);
				if(direction > 0) {
					if(_mult > 0.0)
						_data.Set.Qtty += _mult;
					else if(_upp > 0.0)
						_data.Set.Qtty += _upp;
					else
						_data.Set.Qtty += 1.0;
					do_update = true;
				}
				else if(direction < 0) {
					double decrement = 0.0;
					if(_mult > 0.0)
						decrement = _mult;
					else if(_upp > 0.0)
						decrement = _upp;
					else
						decrement = 1.0;
					if(_data.Set.Qtty >= decrement) {
						_data.Set.Qtty -= decrement;
						do_update = true;
					}
				}
				if(do_update) {
					SLib.SetCtrlString(this, R.id.CTL_ORDRTI_QTTY, SLib.formatdouble(_data.Set.Qtty, 3));
					SLib.SetCtrlString(this, R.id.CTL_ORDRTI_AMOUNT, ActivityCtx.CPM.FormatCurrency(_data.Set.Qtty * _data.Set.Price));
				}
			}
		}
		@Override public Object HandleEvent(int ev, Object srcObj, Object subj)
		{
			Object result = null;
			switch(ev) {
				case SLib.EV_CREATE:
					requestWindowFeature(Window.FEATURE_NO_TITLE);
					setContentView(R.layout.dialog_ordrti);
					SetDTS(Data);
					break;
				case SLib.EV_COMMAND:
					/*{
						Context ctx = getContext();
						StyloQApp app_ctx = (StyloQApp)ctx.getApplicationContext();
						app_ctx.DisplayMessage(ctx, "debug ev_command", 10000);
					}*/
					if(srcObj != null && srcObj instanceof View) {
						final int view_id = ((View)srcObj).getId();
						if(view_id == R.id.STDCTL_OKBUTTON) {
							Object data = GetDTS();
							if(data != null) {
								Context ctx = getContext();
								StyloQApp app_ctx = (StyloQApp)ctx.getApplicationContext();
								if(app_ctx != null)
									app_ctx.HandleEvent(SLib.EV_IADATAEDITCOMMIT, this, data);
							}
							this.dismiss();
						}
						else if(view_id == R.id.STDCTL_CANCELBUTTON) {
							this.dismiss();
						}
						else if(view_id == R.id.STDCTL_DELETEBUTTON) {
							if(Data != null && Data instanceof Document.TransferItem) {
								Context ctx = getContext();
								StyloQApp app_ctx = (ctx != null) ? (StyloQApp) ctx.getApplicationContext() : null;
								if(app_ctx != null) {
									/*
									try {
										StyloQDatabase db = app_ctx.GetDB();
										db.DeleteForeignSvc(((StyloQDatabase.SecStoragePacket)Data).Rec.ID);
										app_ctx.HandleEvent(SLib.EV_IADATADELETECOMMIT, this, Data);
										this.dismiss(); // Close Dialog
									} catch(StyloQException exn) {
										;
									}
									 */
								}
							}
						}
						else if(view_id == R.id.CTL_QTTY_UP) {
							StepQuantity(+1);
						}
						else if(view_id == R.id.CTL_QTTY_DN) {
							StepQuantity(-1);
						}
					}
					break;
			}
			return result;
		}
		boolean SetDTS(Object objData)
		{
			boolean ok = true;
			if(objData != null && objData.getClass() == Data.getClass()) {
				Context ctx = getContext();
				StyloQApp app_ctx = (ctx != null) ? (StyloQApp)ctx.getApplicationContext() : null;
				if(app_ctx != null) {
					Document.TransferItem _data = null;
					CommonPrereqModule.WareEntry goods_item = null;
					if(Data != null && Data instanceof Document.TransferItem)
						_data = (Document.TransferItem)Data;
					else {
						Data = new Document.TransferItem();
						_data = (Document.TransferItem)Data;
					}
					double _upp = 0.0; // Емкость упаковки
					double _mult = 0.0; // Кратность количества в заказе
					double _min = 0.0;	// Минимальный заказ
					String text = "";
					String blob_signature = null;
					if(_data != null && _data.GoodsID > 0 && ActivityCtx != null) {
						goods_item = ActivityCtx.CPM.FindGoodsItemByGoodsID(_data.GoodsID);
						if(goods_item != null && goods_item.JsItem != null) {
							text = goods_item.JsItem.optString("nm", "");
							blob_signature =  goods_item.JsItem.optString("imgblobs", null);
							//
							_upp = goods_item.JsItem.optDouble("upp", 0.0);
							_mult = goods_item.JsItem.optDouble("ordqtymult", 0.0);
							_min = goods_item.JsItem.optDouble("ordminqty", 0.0);
						}
					}
					if(_upp > 0.0) {
						String upp_text = app_ctx.GetString("unitperpack_ss") + " " + SLib.formatdouble(_upp, 0);
						SLib.SetCtrlString(this, R.id.CTL_ORDRTI_PACK, upp_text);
					}
					else {
						SLib.SetCtrlVisibility(this, R.id.CTL_ORDRTI_PACK, View.GONE);
					}
					if(_mult > 0.0) {
						String upp_text = app_ctx.GetString("goods_fmultminshipm_ss") + " " + SLib.formatdouble(_mult, 0);
						SLib.SetCtrlString(this, R.id.CTL_ORDRTI_MINORMULT, upp_text);
					}
					else if(_min > 0.0) {
						String upp_text = app_ctx.GetString("goods_minshippmqtty_ss") + " " + SLib.formatdouble(_min, 0);
						SLib.SetCtrlString(this, R.id.CTL_ORDRTI_MINORMULT, upp_text);
					}
					else {
						SLib.SetCtrlVisibility(this, R.id.CTL_ORDRTI_MINORMULT, View.GONE);
					}
					SLib.SetCtrlString(this, R.id.CTL_ORDRTI_GOODSNAME, text);
					SLib.SetCtrlString(this, R.id.CTL_ORDRTI_PRICE, ActivityCtx.CPM.FormatCurrency(_data.Set.Price));
					SLib.SetCtrlString(this, R.id.CTL_ORDRTI_QTTY, SLib.formatdouble(_data.Set.Qtty, 3));
					SLib.SetCtrlString(this, R.id.CTL_ORDRTI_AMOUNT, ActivityCtx.CPM.FormatCurrency(_data.Set.Qtty * _data.Set.Price));
					//
					SLib.SetupImage(ActivityCtx, this.findViewById(R.id.CTL_ORDRTI_IMG), blob_signature);
				}
			}
			return ok;
		}
		Object GetDTS()
		{
			Object result = null;
			Context ctx = getContext();
			StyloQApp app_ctx = (ctx != null) ? (StyloQApp)ctx.getApplicationContext() : null;
			if(app_ctx != null) {
				Document.TransferItem _data = null;
				if(Data != null && Data instanceof Document.TransferItem)
					_data = (Document.TransferItem)Data;
				else {
					_data = new Document.TransferItem();
					Data = _data;
				}
				String qtty_text = SLib.GetCtrlString(this, R.id.CTL_ORDRTI_QTTY);
				if(SLib.GetLen(qtty_text) > 0)
					_data.Set.Qtty = Double.parseDouble(qtty_text);
				result = Data;
			}
			return result;
		}
	}
	/*private boolean CommitCurrentDocument()
	{
		boolean ok = false;
		if(!Locker_CommitCurrentDocument) {
			Locker_CommitCurrentDocument = true;
			StyloQApp app_ctx = (StyloQApp) getApplicationContext();
			if(app_ctx != null) {
				if(CPM.CurrentOrder != null && CPM.CurrentOrder.Finalize()) {
					StyloQApp.PostDocumentResult result = app_ctx.RunSvcPostDocumentCommand(CPM.SvcIdent, CPM.CurrentOrder, this);
					ok = result.PostResult;
					if(ok) {
						;
					}
				}
			}
		}
		return ok;
	}*/
	int FindClientItemIndexByID(int id)
	{
		int result = -1;
		if(CliListData != null && id > 0) {
			for(int i = 0; result < 0 && i < CliListData.size(); i++) {
				final int iter_id = CliListData.get(i).JsItem.optInt("id", 0);
				if(iter_id == id)
					result = i;
			}
		}
		return result;
	}
	int FindBrandItemIndexByID(int id)
	{
		int result = -1;
		if(BrandListData != null && id > 0) {
			for(int i = 0; result < 0 && i < BrandListData.size(); i++) {
				final int iter_id = BrandListData.get(i).optInt("id", 0);
				if(iter_id == id)
					result = i;
			}
		}
		return result;
	}
	private void CreateTabList(boolean force)
	{
		final int tab_layout_rcid = R.id.TABLAYOUT_ORDERPREREQ;
		StyloQApp app_ctx = (StyloQApp)getApplicationContext();
		if(app_ctx != null && (CPM.TabList == null || force)) {
			CPM.TabList = new ArrayList<CommonPrereqModule.TabEntry>();
			LayoutInflater inflater = LayoutInflater.from(this);
			if(CPM.GoodsGroupListData != null) {
				final CommonPrereqModule.Tab _tab = CommonPrereqModule.Tab.tabGoodsGroups;
				SLib.SlFragmentStatic f = SLib.SlFragmentStatic.newInstance(_tab.ordinal(), R.layout.layout_orderprereq_goodsgroups, tab_layout_rcid);
				CPM.TabList.add(new CommonPrereqModule.TabEntry(_tab, SLib.ExpandString(app_ctx, "@{group_pl}"), f));
			}
			if(BrandListData != null) {
				final CommonPrereqModule.Tab _tab = CommonPrereqModule.Tab.tabBrands;
				SLib.SlFragmentStatic f = SLib.SlFragmentStatic.newInstance(_tab.ordinal(), R.layout.layout_orderprereq_brands, tab_layout_rcid);
				CPM.TabList.add(new CommonPrereqModule.TabEntry(_tab, SLib.ExpandString(app_ctx, "@{brand_pl}"), f));
			}
			if(CPM.GoodsListData != null) {
				final CommonPrereqModule.Tab _tab = CommonPrereqModule.Tab.tabGoods;
				SLib.SlFragmentStatic f = SLib.SlFragmentStatic.newInstance(_tab.ordinal(), R.layout.layout_orderprereq_goods, tab_layout_rcid);
				CPM.TabList.add(new CommonPrereqModule.TabEntry(_tab, SLib.ExpandString(app_ctx, "@{ware_pl}"), f));
			}
			if(CliListData != null) {
				final CommonPrereqModule.Tab _tab = CommonPrereqModule.Tab.tabClients;
				SLib.SlFragmentStatic f = SLib.SlFragmentStatic.newInstance(_tab.ordinal(), R.layout.layout_orderprereq_clients, tab_layout_rcid);
				CPM.TabList.add(new CommonPrereqModule.TabEntry(_tab, SLib.ExpandString(app_ctx, "@{client_pl}"), f));
			}
			{
				final CommonPrereqModule.Tab _tab = CommonPrereqModule.Tab.tabCurrentOrder;
				SLib.SlFragmentStatic f = SLib.SlFragmentStatic.newInstance(_tab.ordinal(), R.layout.layout_orderprereq_ordr, tab_layout_rcid);
				CPM.TabList.add(new CommonPrereqModule.TabEntry(_tab, SLib.ExpandString(app_ctx, "@{orderdocument}"), f));
			}
			{
				final CommonPrereqModule.Tab _tab = CommonPrereqModule.Tab.tabOrders;
				SLib.SlFragmentStatic f = SLib.SlFragmentStatic.newInstance(_tab.ordinal(), R.layout.layout_orderprereq_orders, tab_layout_rcid);
				CPM.TabList.add(new CommonPrereqModule.TabEntry(_tab, SLib.ExpandString(app_ctx, "@{booking_pl}"), f));
			}
			{
				final CommonPrereqModule.Tab _tab = CommonPrereqModule.Tab.tabSearch;
				SLib.SlFragmentStatic f = SLib.SlFragmentStatic.newInstance(_tab.ordinal(), R.layout.layout_searchpane, tab_layout_rcid);
				CPM.TabList.add(new CommonPrereqModule.TabEntry(_tab, SLib.ExpandString(app_ctx, "[search]"), f));
			}
		}
	}
	private CommonPrereqModule.TabEntry SearchTabEntry(CommonPrereqModule.Tab tab)
	{
		CommonPrereqModule.TabEntry result = null;
		if(tab != CommonPrereqModule.Tab.tabUndef) {
			ViewPager2 view_pager = (ViewPager2) findViewById(R.id.VIEWPAGER_ORDERPREREQ);
			if(view_pager != null) {
				for(int tidx = 0; tidx < CPM.TabList.size(); tidx++) {
					if(CPM.TabList.get(tidx).TabId == tab)
						result = CPM.TabList.get(tidx);
				}
			}
		}
		return result;
	}
	private void GotoTab(CommonPrereqModule.Tab tab, @IdRes int recyclerViewToUpdate, int goToIndex, int nestedIndex)
	{
		if(tab != CommonPrereqModule.Tab.tabUndef) {
			ViewPager2 view_pager = (ViewPager2)findViewById(R.id.VIEWPAGER_ORDERPREREQ);
			if(view_pager != null) {
				for(int tidx = 0; tidx < CPM.TabList.size(); tidx++) {
					final CommonPrereqModule.TabEntry te = CPM.TabList.get(tidx);
					if(te.TabId == tab) {
						SLib.SlFragmentStatic f = te.TabView;
						if(f != null) {
							view_pager.setCurrentItem(tidx);
							if(recyclerViewToUpdate != 0) {
								View fv2 = view_pager.getChildAt(tidx);
								//f.requireView();
								View fv = f.getView();
								if(fv != null && fv instanceof ViewGroup) {
									View lv = fv.findViewById(recyclerViewToUpdate);
									if(lv != null && lv instanceof RecyclerView) {
										RecyclerView.Adapter gva = ((RecyclerView)lv).getAdapter();
										if(gva != null) {
											if(goToIndex >= 0 && goToIndex < gva.getItemCount()) {
												SetRecyclerListFocusedIndex(gva, goToIndex);
												((RecyclerView)lv).scrollToPosition(goToIndex);
											}
											gva.notifyDataSetChanged();
										}
									}
								}
							}
						}
						break;
					}
				}
			}
		}
	}
	private void NotifyTabContentChanged(CommonPrereqModule.Tab tabId, int innerViewId)
	{
		ViewPager2 view_pager = (ViewPager2)findViewById(R.id.VIEWPAGER_ORDERPREREQ);
		if(view_pager != null && CPM.TabList != null) {
			for(int tidx = 0; tidx < CPM.TabList.size(); tidx++) {
				if(CPM.TabList.get(tidx).TabId == tabId) {
					SLib.SlFragmentStatic f = CPM.TabList.get(tidx).TabView;
					if(f != null) {
						View fv = f.getView();
						if(fv != null) {
							View lv = (innerViewId != 0) ? fv.findViewById(innerViewId) : null;
							if(lv != null) {
								if(lv instanceof RecyclerView) {
									RecyclerView.Adapter gva = ((RecyclerView) lv).getAdapter();
									if(gva != null)
										gva.notifyDataSetChanged();
								}
								else {
									lv.refreshDrawableState();
								}
							}
						}
					}
					break;
				}
			}
		}
	}
	private void NotifyDocListChanged() { NotifyTabContentChanged(CommonPrereqModule.Tab.tabOrders, R.id.orderPrereqOrderListView); }
	private void NotifyCurrentOrderChanged()
	{
		NotifyTabContentChanged(CommonPrereqModule.Tab.tabCurrentOrder, R.id.orderPrereqOrdrListView);
		//NotifyTabContentChanged(CommonPrereqModule.Tab.tabCurrentOrder, R.id.CTL_DOCUMENT_AMOUNT);
		CommonPrereqModule.TabEntry tab_entry = SearchTabEntry(CommonPrereqModule.Tab.tabCurrentOrder);
		if(tab_entry != null && tab_entry.TabView != null)
			HandleEvent(SLib.EV_SETVIEWDATA, tab_entry.TabView.getView(), null);
	}
	private JSONObject FindClientEntry(int cliID)
	{
		JSONObject result = null;
		if(CliListData != null && cliID > 0) {
			for(int i = 0; i < CliListData.size(); i++) {
				CliEntry ce = CliListData.get(i);
				if(ce != null && ce.JsItem != null) {
					int _id = ce.JsItem.optInt("id", 0);
					if(_id == cliID) {
						result = ce.JsItem;
						break;
					}
				}
			}
		}
		return result;
	}
	private JSONObject FindDlvrLocEntryInCliEntry(JSONObject cliJs, int dlvrLocID)
	{
		JSONObject result = null;
		try {
			if(CliListData != null && cliJs != null && dlvrLocID > 0) {
				JSONArray dvlrloc_list_js = cliJs.optJSONArray("dlvrloc_list");
				if(dvlrloc_list_js != null && dvlrloc_list_js.length() > 0) {
					for(int j = 0; j < dvlrloc_list_js.length(); j++) {
						JSONObject dlvrloc_js = dvlrloc_list_js.getJSONObject(j);
						if(dlvrloc_js != null) {
							int iter_id = dlvrloc_js.optInt("id", 0);
							if(iter_id == dlvrLocID) {
								result = dlvrloc_js;
								break;
							}
						}
					}
				}
			}
		} catch(JSONException exn) {
			result = null;
		}
		return result;
	}
	private int FindDlvrLocEntryIndexInCliEntry(JSONObject cliJs, int dlvrLocID)
	{
		int result = -1;
		try {
			if(CliListData != null && cliJs != null && dlvrLocID > 0) {
				JSONArray dvlrloc_list_js = cliJs.optJSONArray("dlvrloc_list");
				if(dvlrloc_list_js != null && dvlrloc_list_js.length() > 0) {
					for(int j = 0; j < dvlrloc_list_js.length(); j++) {
						JSONObject dlvrloc_js = dvlrloc_list_js.getJSONObject(j);
						if(dlvrloc_js != null) {
							int iter_id = dlvrloc_js.optInt("id", 0);
							if(iter_id == dlvrLocID) {
								result = j;
								break;
							}
						}
					}
				}
			}
		} catch(JSONException exn) {
			result = -1;
		}
		return result;
	}
	private JSONObject FindClientEntryByDlvrLocID(int dlvrLocID)
	{
		JSONObject result = null;
		if(CliListData != null && dlvrLocID > 0) {
			for(int i = 0; result == null && i < CliListData.size(); i++) {
				CliEntry ce = CliListData.get(i);
				if(ce != null && ce.JsItem != null) {
					if(FindDlvrLocEntryInCliEntry(ce.JsItem, dlvrLocID) != null)
						result = ce.JsItem;
				}
			}
		}
		return result;
	}
	private boolean SetCurrentOrderClient(JSONObject cliItem, JSONObject dlvrLocItem)
	{
		boolean result = false;
		try {
			JSONObject final_cli_js = null;
			if(CliListData != null) {
				if(dlvrLocItem != null) {
					final_cli_js = FindClientEntryByDlvrLocID(dlvrLocItem.optInt("id", 0));
				}
				else if(cliItem != null) {
					final_cli_js = cliItem;
					int cli_id = cliItem.optInt("id", 0);
					if(cli_id > 0) {

					}
				}
				if(final_cli_js != null) {
					int cli_id = final_cli_js.optInt("id", 0);
					int dlvrloc_id = (dlvrLocItem != null) ? dlvrLocItem.optInt("id", 0) : 0;
					result = CPM.SetClientToCurrentDocument((StyloQApp)getApplicationContext(), cli_id, dlvrloc_id);
					if(result) {
						CPM.SetTabVisibility(CommonPrereqModule.Tab.tabCurrentOrder, View.VISIBLE);
						NotifyCurrentOrderChanged();
						NotifyTabContentChanged(CommonPrereqModule.Tab.tabClients, R.id.orderPrereqClientsListView);
					}
				}
			}
		} catch(StyloQException e) {
			result = false;
		}
		return result;
	}
	//
	private boolean AddItemToCurrentOrder(Document.TransferItem item)
	{
		boolean result = false;
		try {
			if(item != null && item.GoodsID > 0 && item.Set != null && item.Set.Qtty > 0.0) {
				CommonPrereqModule.WareEntry goods_item = CPM.FindGoodsItemByGoodsID(item.GoodsID);
				double price = goods_item.JsItem.optDouble("price", 0.0);
				if(CPM.CurrentOrder == null) {
					CPM.CurrentOrder = new Document(SLib.PPEDIOP_ORDER, CPM.SvcIdent, (StyloQApp)getApplicationContext());
					CPM.CurrentOrder.H.BaseCurrencySymb = CPM.GetBaseCurrencySymb();
				}
				Document.TransferItem ti = item;
				ti.Set.Price = price;
				int max_row_idx = 0;
				boolean merged = false;
				if(CPM.CurrentOrder.TiList != null) {
					for(int i = 0; !merged && i < CPM.CurrentOrder.TiList.size(); i++) {
						Document.TransferItem iter_ti = CPM.CurrentOrder.TiList.get(i);
						merged = iter_ti.Merge(ti);
						if(max_row_idx < iter_ti.RowIdx)
							max_row_idx = iter_ti.RowIdx;
					}
				}
				if(!merged) {
					ti.RowIdx = max_row_idx + 1;
					if(CPM.CurrentOrder.TiList == null)
						CPM.CurrentOrder.TiList = new ArrayList<Document.TransferItem>();
					CPM.CurrentOrder.TiList.add(ti);
				}
				CPM.SetTabVisibility(CommonPrereqModule.Tab.tabCurrentOrder, View.VISIBLE);
				NotifyCurrentOrderChanged();
				result = true;
			}
		} catch(StyloQException exn) {
			;
		}
		return result;
	}
	private static class DlvrLocListAdapter extends ArrayAdapter {
		private int RcId;
		DlvrLocListAdapter(Context ctx, int rcId, ArrayList data)
		{
			super(ctx, rcId, data);
			RcId = rcId;
		}
		@Override
		public View getView(int position, View convertView, ViewGroup parent)
		{
			// Get the data item for this position
			Object item = (Object)getItem(position);
			//Context ctx = parent.getContext();
			if(item != null) {
				// Check if an existing view is being reused, otherwise inflate the view
				if(convertView == null) {
					Context _ctx = getContext();
					convertView = LayoutInflater.from(_ctx).inflate(RcId, parent, false);
				}
				if(convertView != null) {
					TextView v = convertView.findViewById(R.id.LVITEM_GENERICNAME);
					if(v != null) {
						if(item instanceof JSONObject) {
							JSONObject js_item = (JSONObject)item;
							v.setText(js_item.optString("addr", ""));
						}
					}
				}
			}
			return convertView; // Return the completed view to render on screen
		}
	}
	private void SetListBackground(View iv, SLib.RecyclerListAdapter a, int itemIdxToDraw, int objType, int objID)
	{
		int shaperc = 0;
		if(GetRecyclerListFocusedIndex(a) == itemIdxToDraw)
			shaperc = R.drawable.shape_listitem_focused;
		else {
			boolean is_catched = false;
			if(objID > 0 && CPM.CurrentOrder != null) {
				if(CPM.CurrentOrder.H != null) {
					if(objType == SLib.PPOBJ_PERSON && objID == CPM.CurrentOrder.H.ClientID) {
						is_catched = true;
					}
					else if(objType == SLib.PPOBJ_LOCATION && objID == CPM.CurrentOrder.H.DlvrLocID) {
						is_catched = true;
					}
					else if(objType == SLib.PPOBJ_GOODS) {
						if(CPM.HasGoodsInCurrentOrder(objID))
							is_catched = true;
					}
				}
			}
			if(is_catched)
				shaperc = R.drawable.shape_listitem_catched;
			else if(CPM.IsObjInSearchResult(objType, objID))
				shaperc = R.drawable.shape_listitem_found;
			else
				shaperc = R.drawable.shape_listitem;
		}
		iv.setBackground(getResources().getDrawable(shaperc, getTheme()));
	}
	private void GetFragmentData(Object entry)
	{
		if(entry != null) {
			ViewGroup vg = null;
			if(entry instanceof SLib.SlFragmentStatic) {
				View v = ((SLib.SlFragmentStatic)entry).getView();
				if(v instanceof ViewGroup)
					vg = (ViewGroup)v;
			}
			else if(entry instanceof ViewGroup)
				vg = (ViewGroup)entry;
			if(vg != null) {
				int vg_id = vg.getId();
				if(vg_id == R.id.LAYOUT_ORDERPREPREQ_ORDR) {
					StyloQApp app_ctx = (StyloQApp)getApplicationContext();
					if(CPM.CurrentOrder != null && CPM.CurrentOrder.H != null) {
						CPM.CurrentOrder.H.Memo = SLib.GetCtrlString(vg, R.id.CTL_DOCUMENT_MEMO);
					}
				}
			}
		}
	}
	public Object HandleEvent(int ev, Object srcObj, Object subj)
	{
		Object result = null;
		switch(ev) {
			case SLib.EV_CREATE:
				{
					Intent intent = getIntent();
					try {
						CPM.GetAttributesFromIntent(intent);
						long doc_id = intent.getLongExtra("SvcReplyDocID", 0);
						String svc_reply_doc_json = null;
						StyloQApp app_ctx = (StyloQApp)getApplication();
						StyloQDatabase db = app_ctx.GetDB();
						if(doc_id > 0) {
							StyloQDatabase.SecStoragePacket doc_packet = db.GetPeerEntry(doc_id);
							if(doc_packet != null) {
								byte [] raw_doc = doc_packet.Pool.Get(SecretTagPool.tagRawData);
								if(SLib.GetLen(raw_doc) > 0)
									svc_reply_doc_json = new String(raw_doc);
							}
						}
						else
							svc_reply_doc_json = intent.getStringExtra("SvcReplyDocJson");
						if(SLib.GetLen(svc_reply_doc_json) > 0) {
							JSONObject js_head = new JSONObject(svc_reply_doc_json);
							CPM.GetCommonJsonFactors(js_head);
							UomListData = js_head.optJSONArray("uom_list");
							CPM.MakeGoodsGroupListFromCommonJson(js_head);
							CPM.MakeGoodsListFromCommonJson(js_head);
							{
								JSONArray temp_array = js_head.optJSONArray("brand_list");
								if(temp_array != null) {
									BrandListData = new ArrayList<JSONObject>();
									for(int i = 0; i < temp_array.length(); i++) {
										Object temp_obj = temp_array.get(i);
										if(temp_obj != null && temp_obj instanceof JSONObject)
											BrandListData.add((JSONObject)temp_obj);
									}
									Collections.sort(BrandListData, new Comparator<JSONObject>() {
										@Override public int compare(JSONObject lh, JSONObject rh)
										{
											String ls = lh.optString("nm", "");
											String rs = rh.optString("nm", "");
											return ls.toLowerCase().compareTo(rs.toLowerCase());
										}
									});
								}
							}
							WharehouseListData = js_head.optJSONArray("warehouse_list");
							QuotKindListData = js_head.optJSONArray("quotkind_list");
							{
								JSONArray temp_array = js_head.optJSONArray("client_list");
								if(temp_array != null) {
									CliListData = new ArrayList<CliEntry>();
									for(int i = 0; i < temp_array.length(); i++) {
										Object temp_obj = temp_array.get(i);
										if(temp_obj != null && temp_obj instanceof JSONObject)
											CliListData.add(new CliEntry((JSONObject)temp_obj));
									}
									Collections.sort(CliListData, new Comparator<CliEntry>() {
										@Override public int compare(CliEntry lh, CliEntry rh)
										{
											String ls = lh.JsItem.optString("nm", "");
											String rs = lh.JsItem.optString("nm", "");
											return ls.toLowerCase().compareTo(rs.toLowerCase());
										}
									});
								}
							}
							CPM.MakeCurrentDocList(app_ctx);
							MakeSimpleSearchIndex();
						}
						requestWindowFeature(Window.FEATURE_NO_TITLE);
						setContentView(R.layout.activity_cmdrorderprereq);
						CPM.SetupActivity(db, R.id.VIEWPAGER_ORDERPREREQ, R.id.TABLAYOUT_ORDERPREREQ);
						ViewPager2 view_pager = (ViewPager2)findViewById(R.id.VIEWPAGER_ORDERPREREQ);
						SetupViewPagerWithFragmentAdapter(R.id.VIEWPAGER_ORDERPREREQ);
						{
							TabLayout lo_tab = findViewById(R.id.TABLAYOUT_ORDERPREREQ);
							if(lo_tab != null) {
								CreateTabList(false);
								for(int i = 0; i < CPM.TabList.size(); i++) {
									TabLayout.Tab tab = lo_tab.newTab();
									tab.setText(CPM.TabList.get(i).TabText);
									lo_tab.addTab(tab);
								}
								SLib.SetupTabLayoutStyle(lo_tab);
								SLib.SetupTabLayoutListener(lo_tab, view_pager);
								if(CPM.CurrentOrder == null || CPM.CurrentOrder.H == null) {
									CPM.SetTabVisibility(CommonPrereqModule.Tab.tabCurrentOrder, View.GONE);
								}
								//SetTabVisibility(Tab.tabSearch, View.GONE);
							}
						}
						SLib.SetCtrlVisibility(this, R.id.tbButtonClearFiter, View.GONE);
					} catch(JSONException exn) {
						//exn.printStackTrace();
					} catch(StyloQException exn) {
						//exn.printStackTrace();
					}
				}
				break;
			case SLib.EV_LISTVIEWCOUNT:
				if(srcObj instanceof SLib.FragmentAdapter) {
					CreateTabList(false);
					result = new Integer(CPM.TabList.size());
				}
				else if(srcObj instanceof SLib.RecyclerListAdapter) {
					SLib.RecyclerListAdapter a = (SLib.RecyclerListAdapter)srcObj;
					switch(a.GetListRcId()) {
						case R.id.orderPrereqGoodsListView: result = new Integer(CPM.GetGoodsListSize()); break;
						case R.id.orderPrereqGoodsGroupListView: result = new Integer((CPM.GoodsGroupListData != null) ? CPM.GoodsGroupListData.size() : 0); break;
						case R.id.orderPrereqBrandListView: result = new Integer((BrandListData != null) ? BrandListData.size() : 0); break;
						case R.id.orderPrereqOrdrListView: result = new Integer((CPM.CurrentOrder != null && CPM.CurrentOrder.TiList != null) ? CPM.CurrentOrder.TiList.size() : 0); break;
						case R.id.orderPrereqOrderListView:
							result = new Integer((CPM.OrderHList != null) ? CPM.OrderHList.size() : 0);
							break;
						case R.id.orderPrereqClientsListView: result = new Integer((CliListData != null) ? CliListData.size() : 0); break;
						case R.id.searchPaneListView:
							{
								result = new Integer((CPM.SearchResult != null) ? CPM.SearchResult.GetObjTypeCount() : 0);
								//result = new Integer((SearchResult != null && SearchResult.List != null) ? SearchResult.List.size() : 0);
							}
							break;
					}
				}
				break;
			case SLib.EV_GETVIEWDATA:
				if(srcObj != null && srcObj instanceof ViewGroup)
					GetFragmentData(srcObj);
				break;
			case SLib.EV_SETVIEWDATA:
				if(srcObj != null && srcObj instanceof ViewGroup) {
					ViewGroup vg = (ViewGroup)srcObj;
					int vg_id = vg.getId();
					if(vg_id == R.id.LAYOUT_ORDERPREPREQ_ORDR) {
						if(CPM.CurrentOrder == null || CPM.CurrentOrder.H == null) {
							SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_CODE, "");
							SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_DATE, "");
							SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_CLI, "");
							SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_DLVRLOC, "");
							SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_AMOUNT, "");
							SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_MEMO, "");
						}
						else {
							if(SLib.GetLen(CPM.CurrentOrder.H.Code) > 0)
								SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_CODE, CPM.CurrentOrder.H.Code);
							SLib.LDATE d = null;
							if(CPM.CurrentOrder.H.Time > 0)
								d = SLib.BuildDateByEpoch(CPM.CurrentOrder.H.Time);
							else if(CPM.CurrentOrder.H.CreationTime > 0)
								d = SLib.BuildDateByEpoch(CPM.CurrentOrder.H.CreationTime);
							if(d != null) {
								String ds = d.Format(SLib.DATF_ISO8601|SLib.DATF_CENTURY);
								SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_DATE, ds);
							}
							{
								String cli_name = "";
								String addr = "";
								if(CPM.CurrentOrder.H.ClientID > 0) {
									JSONObject cli_entry = FindClientEntry(CPM.CurrentOrder.H.ClientID);
									if(cli_entry != null)
										cli_name = cli_entry.optString("nm", "");
								}
								if(CPM.CurrentOrder.H.DlvrLocID > 0) {
									JSONObject cli_entry = FindClientEntryByDlvrLocID(CPM.CurrentOrder.H.DlvrLocID);
									if(cli_entry != null) {
										JSONObject dlvrlov_entry = FindDlvrLocEntryInCliEntry(cli_entry, CPM.CurrentOrder.H.DlvrLocID);
										if(dlvrlov_entry != null)
											addr = dlvrlov_entry.optString("addr", "");
									}
								}
								SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_CLI, cli_name);
								SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_DLVRLOC, addr);
							}
							SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_MEMO, CPM.CurrentOrder.H.Memo);
							{
								double amount = CPM.GetAmountOfCurrentDocument();
								SLib.SetCtrlString(vg, R.id.CTL_DOCUMENT_AMOUNT, CPM.FormatCurrency(amount));
							}
						}
					}
				}
				break;
			case SLib.EV_GETLISTITEMVIEW:
				{
					SLib.ListViewEvent ev_subj = (subj instanceof SLib.ListViewEvent) ? (SLib.ListViewEvent) subj : null;
					if(ev_subj != null && ev_subj.ItemIdx >= 0) {
						if(ev_subj.RvHolder != null) {
							// RecyclerView
							if(srcObj != null && srcObj instanceof SLib.RecyclerListAdapter) {
								SLib.RecyclerListAdapter a = (SLib.RecyclerListAdapter)srcObj;
								if(a.GetListRcId() == R.id.searchPaneListView) {
									CPM.GetSearchPaneListViewItem((StyloQApp)getApplicationContext(), ev_subj.RvHolder.itemView, ev_subj.ItemIdx);
								}
								else if(a.GetListRcId() == R.id.orderPrereqClientsListView) {
									if(CliListData != null && ev_subj.ItemIdx < CliListData.size()) {
										View iv = ev_subj.RvHolder.itemView;
										CliEntry cur_entry = null;
										cur_entry = (CliEntry)CliListData.get(ev_subj.ItemIdx);
										final int cur_cli_id = cur_entry.JsItem.optInt("id", 0);
										SLib.SetCtrlString(iv, R.id.LVITEM_GENERICNAME, cur_entry.JsItem.optString("nm", ""));
										SetListBackground(iv, a, ev_subj.ItemIdx, SLib.PPOBJ_PERSON, cur_cli_id);
										{
											ImageView ctl = (ImageView)iv.findViewById(R.id.ORDERPREREQ_CLI_EXPANDSTATUS);
											if(ctl != null) {
												ListView dlvrloc_lv = (ListView)iv.findViewById(R.id.dlvrLocListView);
												ArrayList <JSONObject> dlvr_loc_list = cur_entry.GetDlvrLocListAsArray();
												if(cur_entry.AddrExpandStatus == 0 || dlvr_loc_list == null) {
													ctl.setVisibility(View.GONE);
													if(dlvrloc_lv != null)
														dlvrloc_lv.setVisibility(View.GONE);
												}
												else if(cur_entry.AddrExpandStatus == 1) {
													ctl.setVisibility(View.VISIBLE);
													ctl.setImageResource(R.drawable.ic_triangleleft03);
													if(dlvrloc_lv != null)
														dlvrloc_lv.setVisibility(View.GONE);
												}
												else if(cur_entry.AddrExpandStatus == 2) {
													ctl.setVisibility(View.VISIBLE);
													ctl.setImageResource(R.drawable.ic_triangledown03);
													if(dlvrloc_lv != null) {
														dlvrloc_lv.setVisibility(View.VISIBLE);
														DlvrLocListAdapter adapter = new DlvrLocListAdapter(/*this*/iv.getContext(), R.layout.li_orderprereq_dlvrloc, dlvr_loc_list);
														dlvrloc_lv.setAdapter(adapter);
														{
															int total_items_height = SLib.CalcListViewHeight(dlvrloc_lv);
															if(total_items_height > 0) {
																ViewGroup.LayoutParams params = dlvrloc_lv.getLayoutParams();
																params.height = total_items_height;
																dlvrloc_lv.setLayoutParams(params);
																dlvrloc_lv.requestLayout();
															}
														}
														adapter.setNotifyOnChange(true);
														dlvrloc_lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
															@Override public void onItemClick(AdapterView<?> parent, View view, int position, long id)
															{
																Object item = (Object)parent.getItemAtPosition(position);
																Context ctx = parent.getContext();
																if(item != null && ctx != null && ctx instanceof SLib.SlActivity) {
																	SLib.SlActivity activity = (SLib.SlActivity)parent.getContext();
																	SLib.ListViewEvent ev_subj = new SLib.ListViewEvent();
																	ev_subj.ItemIdx = position;
																	ev_subj.ItemId = id;
																	ev_subj.ItemObj = item;
																	ev_subj.ItemView = view;
																	//ev_subj.ParentView = parent;
																	activity.HandleEvent(SLib.EV_LISTVIEWITEMCLK, parent, ev_subj);
																}
															}
														});
													}
												}
											}
										}
									}
								}
								else if(a.GetListRcId() == R.id.orderPrereqGoodsListView) {
									CommonPrereqModule.WareEntry cur_entry = CPM.GetGoodsListItemByIdx(ev_subj.ItemIdx);
									if(cur_entry != null && cur_entry.JsItem != null) {
										final int cur_id = cur_entry.JsItem.optInt("id", 0);
										View iv = ev_subj.RvHolder.itemView;
										SLib.SetCtrlString(iv, R.id.LVITEM_GENERICNAME, cur_entry.JsItem.optString("nm", ""));
										{
											double val = cur_entry.JsItem.optDouble("price", 0.0);
											SLib.SetCtrlString(iv, R.id.ORDERPREREQ_GOODS_PRICE, (val > 0.0) ? CPM.FormatCurrency(val) : "");
										}
										{
											double val = cur_entry.JsItem.optDouble("stock", 0.0);
											SLib.SetCtrlString(iv, R.id.ORDERPREREQ_GOODS_REST, (val > 0.0) ? SLib.formatdouble(val, 0) : "");
										}
										{
											double val = CPM.GetGoodsQttyInCurrentDocument(cur_id);
											SLib.SetCtrlString(iv, R.id.ORDERPREREQ_GOODS_ORDEREDQTY, (val > 0.0) ? String.format("%.0f", val) : null);
										}
										String blob_signature = cur_entry.JsItem.optString("imgblobs", null);
										SLib.SetupImage(this, iv.findViewById(R.id.ORDERPREREQ_GOODS_IMG), blob_signature);
										SetListBackground(iv, a, ev_subj.ItemIdx, SLib.PPOBJ_GOODS, cur_id);
									}
								}
								else if(a.GetListRcId() == R.id.orderPrereqGoodsGroupListView) {
									if(CPM.GoodsGroupListData != null && ev_subj.ItemIdx < CPM.GoodsGroupListData.size()) {
										View iv = ev_subj.RvHolder.itemView;
										JSONObject cur_entry = (JSONObject)CPM.GoodsGroupListData.get(ev_subj.ItemIdx);
										SLib.SetCtrlString(iv, R.id.LVITEM_GENERICNAME, cur_entry.optString("nm", ""));
										SetListBackground(iv, a, ev_subj.ItemIdx, SLib.PPOBJ_GOODSGROUP, cur_entry.optInt("id", 0));
									}
								}
								else if(a.GetListRcId() == R.id.orderPrereqBrandListView) {
									if(BrandListData != null && ev_subj.ItemIdx < BrandListData.size()) {
										View iv = ev_subj.RvHolder.itemView;
										JSONObject cur_entry = (JSONObject)BrandListData.get(ev_subj.ItemIdx);
										if(cur_entry != null) {
											SLib.SetCtrlString(iv, R.id.LVITEM_GENERICNAME, cur_entry.optString("nm", ""));
											SetListBackground(iv, a, ev_subj.ItemIdx, SLib.PPOBJ_BRAND, cur_entry.optInt("id", 0));
										}
									}
								}
								else if(a.GetListRcId() == R.id.orderPrereqOrderListView) { // Список зафиксированных заказов
									if(CPM.OrderHList != null && ev_subj.ItemIdx < CPM.OrderHList.size()) {
										View iv = ev_subj.RvHolder.itemView;
										Document.DisplayEntry cur_entry = CPM.OrderHList.get(ev_subj.ItemIdx);
										if(cur_entry != null && cur_entry.H != null) {
											final int _vdlc = VdlDocs.GetCount();
											for(int i = 0; i < _vdlc; i++) {
												TextView ctl = (TextView)iv.findViewById(i+1);
												if(ctl != null) {
													ViewDescriptionList.Item di = VdlDocs.Get(i);
													if(di != null) {
														String text = null;
														if(i == 0) { // date
															SLib.LDATE d = null;
															if(cur_entry.H.Time > 0)
																d = SLib.BuildDateByEpoch(cur_entry.H.Time);
															else if(cur_entry.H.CreationTime > 0)
																d = SLib.BuildDateByEpoch(cur_entry.H.CreationTime);
															if(d != null)
																text = d.Format(SLib.DATF_DMY);
														}
														else if(i == 1) { // code
															text = cur_entry.H.Code;
														}
														else if(i == 2) { // amount
															text = CPM.FormatCurrency(cur_entry.H.Amount);
														}
														else if(i == 3) { // client
															if(cur_entry.H.ClientID > 0) {
																JSONObject cli_entry = FindClientEntry(cur_entry.H.ClientID);
																if(cli_entry != null)
																	text = cli_entry.optString("nm", "");
															}
														}
														ctl.setText(text);
													}
												}
											}
											/*{
												SLib.SetCtrlString(iv, R.id.CTL_DOCUMENT_CODE, (SLib.GetLen(cur_entry.Code) > 0) ? cur_entry.Code : "");
												{
													SLib.LDATE d = null;
													if(cur_entry.Time > 0)
														d = SLib.BuildDateByEpoch(cur_entry.Time);
													else if(cur_entry.CreationTime > 0)
														d = SLib.BuildDateByEpoch(cur_entry.CreationTime);
													if(d != null)
														SLib.SetCtrlString(iv, R.id.CTL_DOCUMENT_DATE, d.Format(SLib.DATF_ISO8601 | SLib.DATF_CENTURY));
												}
												{
													String amount_text = String.format(Locale.US, "%12.2f", cur_entry.Amount);
													SLib.SetCtrlString(iv, R.id.CTL_DOCUMENT_AMOUNT, amount_text);
												}
											}*/
										}
									}
								}
								else if(a.GetListRcId() == R.id.orderPrereqOrdrListView) { // Текущий заказ (точнее, его строки)
									if(CPM.CurrentOrder != null && CPM.CurrentOrder.TiList != null && ev_subj.ItemIdx < CPM.CurrentOrder.TiList.size()) {
										View iv = ev_subj.RvHolder.itemView;
										Document.TransferItem ti = CPM.CurrentOrder.TiList.get(ev_subj.ItemIdx);
										if(ti != null) {
											CommonPrereqModule.WareEntry goods_item = CPM.FindGoodsItemByGoodsID(ti.GoodsID);
											SLib.SetCtrlString(iv, R.id.LVITEM_GENERICNAME, (goods_item != null) ? goods_item.JsItem.optString("nm", "") : "");
											SLib.SetCtrlString(iv, R.id.ORDERPREREQ_TI_PRICE, (ti.Set != null) ? CPM.FormatCurrency(ti.Set.Price) : "");
											SLib.SetCtrlString(iv, R.id.ORDERPREREQ_TI_QTTY, (ti.Set != null) ? SLib.formatdouble(ti.Set.Qtty, 3) : "");
											double item_amont = (ti.Set != null) ? (ti.Set.Qtty * ti.Set.Price) : 0.0;
											SLib.SetCtrlString(iv, R.id.ORDERPREREQ_TI_AMOUNT, " = " + CPM.FormatCurrency(item_amont));
										}
									}
								}
								else if(a.GetListRcId() == R.id.orderPrereqOrderListView) {
								}
							}
						}
						else {
							;
						}
					}
				}
				break;
			case SLib.EV_CREATEFRAGMENT:
				if(subj instanceof Integer) {
					int item_idx = (Integer)subj;
					if(CPM.TabList != null && item_idx >= 0 && item_idx < CPM.TabList.size()) {
						CommonPrereqModule.TabEntry cur_entry = (CommonPrereqModule.TabEntry)CPM.TabList.get(item_idx);
						if(cur_entry.TabView != null)
							result = cur_entry.TabView;
					}
				}
				break;
			case SLib.EV_SETUPFRAGMENT:
				if(subj != null && subj instanceof View) {
					final int selected_search_idx = (CPM.SearchResult != null) ? CPM.SearchResult.GetSelectedItemIndex() : -1;
					final int selected_search_objtype = (selected_search_idx >= 0) ? CPM.SearchResult.List.get(selected_search_idx).ObjType : 0;
					final int selected_search_objid = (selected_search_idx >= 0) ? CPM.SearchResult.List.get(selected_search_idx).ObjID : 0;
					if(srcObj != null && srcObj instanceof SLib.SlFragmentStatic) {
						SLib.SlFragmentStatic fragment = (SLib.SlFragmentStatic)srcObj;
						View fv = (View)subj;
						View lv = fv.findViewById(R.id.orderPrereqGoodsListView);
						if(lv != null) {
							((RecyclerView) lv).setLayoutManager(new LinearLayoutManager(this));
							SetupRecyclerListView(fv, R.id.orderPrereqGoodsListView, R.layout.li_orderprereq_goods);
							if(selected_search_objtype == SLib.PPOBJ_GOODS) {
								final int foc_idx = CPM.FindGoodsItemIndexByID(selected_search_objid);
								SetRecyclerListFocusedIndex(((RecyclerView) lv).getAdapter(), foc_idx);
								SLib.RequestRecyclerListViewPosition((RecyclerView) lv, foc_idx);
								CPM.SearchResult.ResetSelectedItemIndex();
							}
						}
						else {
							lv = fv.findViewById(R.id.orderPrereqOrderListView);
							if(lv != null) {
								StyloQApp app_ctx = (StyloQApp)getApplication();
								SLib.Margin fld_mrgn = new SLib.Margin(8, 2, 8, 2);
								VdlDocs = new ViewDescriptionList();
								{ // #0
									ViewDescriptionList.Item col = new ViewDescriptionList.Item();
									col.Title = app_ctx.GetString("billdate");
									col.StyleRcId = R.style.OrderListItemText;
									col.Mrgn = fld_mrgn;
									VdlDocs.AddItem(col);
								}
								{ // #1
									ViewDescriptionList.Item col = new ViewDescriptionList.Item();
									col.Title = app_ctx.GetString("billno");
									col.TotalFunc = SLib.AGGRFUNC_COUNT;
									col.StyleRcId = R.style.OrderListItemText;
									col.Mrgn = fld_mrgn;
									VdlDocs.AddItem(col);
								}
								{ // #2
									ViewDescriptionList.Item col = new ViewDescriptionList.Item();
									col.Title = app_ctx.GetString("billamount");
									col.TotalFunc = SLib.AGGRFUNC_SUM;
									col.StyleRcId = R.style.OrderListItemText;
									col.Mrgn = fld_mrgn;
									col.ForceAlignment = -1;
									VdlDocs.AddItem(col);
								}
								if(CPM.GetAgentID() > 0) {
									{ // #3
										ViewDescriptionList.Item col = new ViewDescriptionList.Item();
										col.Title = app_ctx.GetString("client");
										col.StyleRcId = R.style.OrderListItemText;
										col.Mrgn = fld_mrgn;
										VdlDocs.AddItem(col);
									}
								}
								if(CPM.OrderHList != null && CPM.OrderHList.size() > 0) {
									final int _vdlc = VdlDocs.GetCount();
									assert(_vdlc > 0);
									for(int i = 0; i < _vdlc; i++) {
										ViewDescriptionList.DataPreprocessBlock dpb = VdlDocs.StartDataPreprocessing(this, i);
										if(dpb != null && dpb.ColumnDescription != null) {
											for(int j = 0; j < CPM.OrderHList.size(); j++) {
												Document.DisplayEntry cur_entry = CPM.OrderHList.get(j);
												if(cur_entry != null && cur_entry.H != null) {
													String text = null;
													if(i == 0) { // date
														SLib.LDATE d = null;
														if(cur_entry.H.Time > 0)
															d = SLib.BuildDateByEpoch(cur_entry.H.Time);
														else if(cur_entry.H.CreationTime > 0)
															d = SLib.BuildDateByEpoch(cur_entry.H.CreationTime);
														if(d != null) {
															VdlDocs.DataPreprocessingIter(dpb, d.Format(SLib.DATF_DMY));
														}
													}
													else if(i == 1) { // code
														VdlDocs.DataPreprocessingIter(dpb, cur_entry.H.Code);
													}
													else if(i == 2) { // amount
														text = CPM.FormatCurrency(cur_entry.H.Amount);
														VdlDocs.DataPreprocessingIter(dpb, new Double(cur_entry.H.Amount), text);
													}
													else if(i == 3) { // client
														if(cur_entry.H.ClientID > 0) {
															JSONObject cli_entry = FindClientEntry(cur_entry.H.ClientID);
															if(cli_entry != null)
																text = cli_entry.optString("nm", "");
														}
														VdlDocs.DataPreprocessingIter(dpb, text);
													}
												}
											}
											VdlDocs.FinishDataProcessing(dpb);
											dpb = null;
										}
									}
									{
										LinearLayout header_layout = (LinearLayout)fv.findViewById(R.id.orderPrereqOrderListHeader);
										if(header_layout != null) {
											LinearLayout _lo_ = ViewDescriptionList.CreateItemLayout(VdlDocs, this, 1);
											if(_lo_ != null)
												header_layout.addView(_lo_);
										}
										if(VdlDocs.IsThereTotals()) {
											LinearLayout bottom_layout = (LinearLayout)fv.findViewById(R.id.orderPrereqOrderListBottom);
											if(bottom_layout != null) {
												LinearLayout _lo_ = ViewDescriptionList.CreateItemLayout(VdlDocs, this,2);
												if(_lo_ != null)
													bottom_layout.addView(_lo_);
											}
										}
									}
								}
								((RecyclerView)lv).setLayoutManager(new LinearLayoutManager(this));
								SetupRecyclerListView(fv, R.id.orderPrereqOrderListView, /*R.layout.li_orderprereq_order*/0);
							}
							else {
								lv = fv.findViewById(R.id.orderPrereqGoodsGroupListView);
								if(lv != null) {
									((RecyclerView) lv).setLayoutManager(new LinearLayoutManager(this));
									SetupRecyclerListView(fv, R.id.orderPrereqGoodsGroupListView, R.layout.li_simple);
									if(selected_search_objtype == SLib.PPOBJ_GOODSGROUP) {
										final int foc_idx = CPM.FindGoodsGroupItemIndexByID(selected_search_objid);
										SetRecyclerListFocusedIndex(((RecyclerView) lv).getAdapter(), foc_idx);
										SLib.RequestRecyclerListViewPosition((RecyclerView) lv, foc_idx);
										CPM.SearchResult.ResetSelectedItemIndex();
									}
								}
								else {
									lv = fv.findViewById(R.id.orderPrereqBrandListView);
									if(lv != null) {
										((RecyclerView) lv).setLayoutManager(new LinearLayoutManager(this));
										SetupRecyclerListView(fv, R.id.orderPrereqBrandListView, R.layout.li_simple);
										if(selected_search_objtype == SLib.PPOBJ_BRAND) {
											final int foc_idx = FindBrandItemIndexByID(selected_search_objid);
											SetRecyclerListFocusedIndex(((RecyclerView) lv).getAdapter(), foc_idx);
											SLib.RequestRecyclerListViewPosition((RecyclerView) lv, foc_idx);
											CPM.SearchResult.ResetSelectedItemIndex();
										}
									}
									else {
										lv = fv.findViewById(R.id.orderPrereqOrdrListView);
										if(lv != null) {
											((RecyclerView) lv).setLayoutManager(new LinearLayoutManager(this));
											SetupRecyclerListView(fv, R.id.orderPrereqOrdrListView, R.layout.li_orderprereq_ordrti);
										}
										else {
											lv = fv.findViewById(R.id.orderPrereqOrderListView);
											if(lv != null) {
												((RecyclerView) lv).setLayoutManager(new LinearLayoutManager(this));
												SetupRecyclerListView(fv, R.id.orderPrereqOrderListView, R.layout.li_orderprereq_order);
											}
											else {
												lv = fv.findViewById(R.id.orderPrereqClientsListView);
												if(lv != null) {
													((RecyclerView) lv).setLayoutManager(new LinearLayoutManager(this));
													SetupRecyclerListView(fv, R.id.orderPrereqClientsListView, R.layout.li_orderprereq_client);
													if(selected_search_objtype == SLib.PPOBJ_PERSON) {
														SLib.RequestRecyclerListViewPosition((RecyclerView) lv, FindClientItemIndexByID(selected_search_objid));
														CPM.SearchResult.ResetSelectedItemIndex();
													}
													else if(selected_search_objtype == SLib.PPOBJ_LOCATION) {
														// @todo
													}
												}
												else {
													lv = fv.findViewById(R.id.searchPaneListView);
													if(lv != null) {
														((RecyclerView) lv).setLayoutManager(new LinearLayoutManager(this));
														SetupRecyclerListView(fv, R.id.searchPaneListView, R.layout.li_searchpane_result);
														{
															View iv = fv.findViewById(R.id.CTL_SEARCHPANE_INPUT);
															if(iv != null && iv instanceof TextInputEditText) {
																TextInputEditText tiv = (TextInputEditText) iv;
																tiv.requestFocus();
																tiv.addTextChangedListener(new TextWatcher() {
																	public void afterTextChanged(Editable s)
																	{
																		//int cross_icon_id = (s.length() > 0) ? R.drawable.ic_cross01 : 0;
																		//tiv.setCompoundDrawablesWithIntrinsicBounds(0, 0, cross_icon_id, 0);
																	}
																	public void beforeTextChanged(CharSequence s, int start, int count, int after)
																	{
																	}
																	public void onTextChanged(CharSequence s, int start, int before, int count)
																	{
																		String pattern = s.toString();
																		StyloQApp app_ctx = (StyloQApp)getApplication();
																		boolean sr = CPM.SearchInSimpleIndex(app_ctx, pattern);
																		String srit = CPM.SearchResult.GetSearchResultInfoText();
																		if(!sr && CPM.SearchResult != null)
																			CPM.SearchResult.Clear();
																		SLib.SetCtrlString(fv, R.id.CTL_SEARCHPANE_RESULTINFO, srit);
																		View lv = findViewById(R.id.searchPaneListView);
																		if(lv != null && lv instanceof RecyclerView) {
																			RecyclerView.Adapter gva = ((RecyclerView) lv).getAdapter();
																			if(gva != null)
																				gva.notifyDataSetChanged();
																		}
																	}
																});
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				break;
			case SLib.EV_CREATEVIEWHOLDER:
				{
					SLib.ListViewEvent ev_subj = (subj != null && subj instanceof SLib.ListViewEvent) ? (SLib.ListViewEvent) subj : null;
					if(ev_subj != null) {
						if(ev_subj.RvHolder == null) {
							if(ev_subj.ItemView != null && ev_subj.ItemView.getId() == R.id.orderPrereqOrderListView) {
								LinearLayout _lo = ViewDescriptionList.CreateItemLayout(VdlDocs, this,0);
								if(_lo != null) {
									SLib.RecyclerListAdapter adapter = (srcObj != null && srcObj instanceof SLib.RecyclerListAdapter) ? (SLib.RecyclerListAdapter)srcObj : null;
									result = new SLib.RecyclerListViewHolder(_lo, adapter);
								}
							}
						}
						else {
							SLib.SetupRecyclerListViewHolderAsClickListener(ev_subj.RvHolder, ev_subj.ItemView, R.id.buttonOrder);
							SLib.SetupRecyclerListViewHolderAsClickListener(ev_subj.RvHolder, ev_subj.ItemView, R.id.ORDERPREREQ_CLI_EXPANDSTATUS);
							result = ev_subj.RvHolder;
						}
					}
					//
				}
				break;
			case SLib.EV_LISTVIEWITEMCLK:
				{
					SLib.ListViewEvent ev_subj = (subj instanceof SLib.ListViewEvent) ? (SLib.ListViewEvent)subj : null;
					if(ev_subj != null && srcObj != null) {
						if(ev_subj.RvHolder == null) {
							if(srcObj instanceof ListView) {
								if(((ListView)srcObj).getId() == R.id.searchPaneTerminalListView) {
									if(ev_subj.ItemObj != null && ev_subj.ItemObj instanceof CommonPrereqModule.SimpleSearchIndexEntry) {
										CommonPrereqModule.SimpleSearchIndexEntry se = (CommonPrereqModule.SimpleSearchIndexEntry)ev_subj.ItemObj;
										// ! ev_subj.ItemIdx не согласуется простым образом с ev_subj.ItemObj из-за
										// двухярусной структуры списка.
										CPM.SearchResult.SetSelectedItemIndex(CPM.SearchResult.FindIndexOfItem(se));
										if(se.ObjType == SLib.PPOBJ_GOODS) {
											int _idx = CPM.FindGoodsItemIndexByID(se.ObjID);
											GotoTab(CommonPrereqModule.Tab.tabGoods, R.id.orderPrereqGoodsListView, _idx, -1);
										}
										else if(se.ObjType == SLib.PPOBJ_PERSON) {
											int _idx = FindClientItemIndexByID(se.ObjID);
											GotoTab(CommonPrereqModule.Tab.tabClients, R.id.orderPrereqClientsListView, _idx, -1);
										}
										else if(se.ObjType == SLib.PPOBJ_LOCATION) {
											JSONObject cli_js_obj = FindClientEntryByDlvrLocID(se.ObjID);
											if(cli_js_obj != null) {
												int cli_id = cli_js_obj.optInt("id", 0);
												if(cli_id > 0) {
													int _idx = FindClientItemIndexByID(cli_id);
													int _dlvr_loc_idx = FindDlvrLocEntryIndexInCliEntry(cli_js_obj, se.ObjID);
													GotoTab(CommonPrereqModule.Tab.tabClients, R.id.orderPrereqClientsListView, _idx, _dlvr_loc_idx);
												}
											}
											//tab_to_select = Tab.tabClients;
										}
										else if(se.ObjType == SLib.PPOBJ_GOODSGROUP) {
											int _idx = CPM.FindGoodsGroupItemIndexByID(se.ObjID);
											GotoTab(CommonPrereqModule.Tab.tabGoodsGroups, R.id.orderPrereqGoodsGroupListView, _idx, -1);
										}
										else if(se.ObjType == SLib.PPOBJ_BRAND) {
											int _idx = FindBrandItemIndexByID(se.ObjID);
											GotoTab(CommonPrereqModule.Tab.tabBrands, R.id.orderPrereqBrandListView, _idx, -1);
										}
									}
								}
								else if(((ListView)srcObj).getId() == R.id.dlvrLocListView) {
									if(ev_subj.ItemObj != null && ev_subj.ItemObj instanceof JSONObject) {
										if(SetCurrentOrderClient(null, (JSONObject)ev_subj.ItemObj)) {
											;
										}
									}
								}
							}
						}
						else if(srcObj instanceof SLib.RecyclerListAdapter) {
							SLib.RecyclerListAdapter a = (SLib.RecyclerListAdapter)srcObj;
							StyloQApp app_ctx = (StyloQApp)getApplication();
							boolean do_update_goods_list_and_toggle_to_it = false;
							if(a.GetListRcId() == R.id.orderPrereqGoodsListView) {
								if(app_ctx != null && ev_subj.ItemIdx >= 0 && ev_subj.ItemIdx < CPM.GetGoodsListSize()) {
									CommonPrereqModule.WareEntry item = CPM.GetGoodsListItemByIdx(ev_subj.ItemIdx);
									if(item != null && ev_subj.ItemView != null && ev_subj.ItemView.getId() == R.id.buttonOrder) {
										final int goods_id = item.JsItem.optInt("id", 0);
										Document.TransferItem ex_ti = CPM.SearchGoodsItemInCurrentOrderTi(goods_id);
										if(ex_ti != null) {
											TransferItemDialog dialog = new TransferItemDialog(this, ex_ti);
											dialog.show();
										}
										else {
											Document.TransferItem ti = new Document.TransferItem();
											if(ti != null) {
												ti.GoodsID = goods_id;
												ti.Set.Price = item.JsItem.optDouble("price", 0.0);
												TransferItemDialog dialog = new TransferItemDialog(this, ti);
												dialog.show();
											}
										}
										/*
										if(AddItemToCurrentOrder(item.JsItem))
											a.notifyItemChanged(ev_subj.ItemIdx);
										 */
									}
								}
							}
							else if(a.GetListRcId() == R.id.orderPrereqClientsListView) {
								if(app_ctx != null && CliListData != null && ev_subj.ItemIdx >= 0 && ev_subj.ItemIdx < CliListData.size()) {
									CliEntry item = CliListData.get(ev_subj.ItemIdx);
									if(item != null && ev_subj.ItemView != null) {
										if(ev_subj.ItemView.getId() == R.id.ORDERPREREQ_CLI_EXPANDSTATUS) {
											// change expand status
											if(item.AddrExpandStatus == 1) {
												item.AddrExpandStatus = 2;
												a.notifyItemChanged(ev_subj.ItemIdx);
											}
											else if(item.AddrExpandStatus == 2) {
												item.AddrExpandStatus = 1;
												a.notifyItemChanged(ev_subj.ItemIdx);
											}
										}
										else {
											// select for order
										}
									}
								}
							}
							else if(a.GetListRcId() == R.id.orderPrereqBrandListView) {
								if(app_ctx != null && BrandListData != null && ev_subj.ItemIdx >= 0 && ev_subj.ItemIdx < BrandListData.size()) {
									final int brand_id = BrandListData.get(ev_subj.ItemIdx).optInt("id", 0);
									if(CPM.SetGoodsFilterByBrand(brand_id)) {
										SLib.SetCtrlVisibility(this, R.id.tbButtonClearFiter, View.VISIBLE);
										do_update_goods_list_and_toggle_to_it = true;
									}
								}
							}
							else if(a.GetListRcId() == R.id.orderPrereqGoodsGroupListView) {
								if(app_ctx != null && CPM.GoodsGroupListData != null && ev_subj.ItemIdx >= 0 && ev_subj.ItemIdx < CPM.GoodsGroupListData.size()) {
									final int group_id = CPM.GoodsGroupListData.get(ev_subj.ItemIdx).optInt("id", 0);
									if(CPM.SetGoodsFilterByGroup(group_id)) {
										SLib.SetCtrlVisibility(this, R.id.tbButtonClearFiter, View.VISIBLE);
										do_update_goods_list_and_toggle_to_it = true;
									}
									//app_ctx.RunSvcCommand(SvcIdent, ListData.Items.get(ev_subj.ItemIdx));
								}
							}
							else if(a.GetListRcId() == R.id.orderPrereqOrdrListView) {
								if(app_ctx != null && CPM.CurrentOrder != null && CPM.CurrentOrder.TiList != null && ev_subj.ItemIdx >= 0 && ev_subj.ItemIdx < CPM.CurrentOrder.TiList.size()) {
									Document.TransferItem ti = CPM.CurrentOrder.TiList.get(ev_subj.ItemIdx);
									if(ti != null) {
										TransferItemDialog dialog = new TransferItemDialog(this, ti);
										dialog.show();
									}
								}
							}
							if(do_update_goods_list_and_toggle_to_it) {
								GotoTab(CommonPrereqModule.Tab.tabGoods, R.id.orderPrereqGoodsListView, -1, -1);
							}
						}
					}
				}
				break;
			case SLib.EV_COMMAND:
				int view_id = View.class.isInstance(srcObj) ? ((View)srcObj).getId() : 0;
				if(view_id == R.id.tbButtonSearch) {
					GotoTab(CommonPrereqModule.Tab.tabSearch, 0, -1, -1);
				}
				else if(view_id == R.id.tbButtonClearFiter) {
					CPM.ResetGoodsFiter();
					SLib.SetCtrlVisibility(this, R.id.tbButtonClearFiter, View.GONE);
					GotoTab(CommonPrereqModule.Tab.tabGoods, R.id.orderPrereqGoodsListView, -1, -1);
				}
				else if(view_id == R.id.STDCTL_COMMITBUTTON) {
					CommonPrereqModule.TabEntry te = SearchTabEntry(CommonPrereqModule.Tab.tabCurrentOrder);
					if(te != null)
						GetFragmentData(te.TabView);
					CPM.CommitCurrentDocument(this);
				}
				break;
			case SLib.EV_IADATAEDITCOMMIT:
				if(srcObj != null && srcObj instanceof TransferItemDialog && subj != null && subj instanceof Document.TransferItem) {
					Document.TransferItem _data = (Document.TransferItem)subj;
					boolean do_notify_goods_list = false;
					if(_data.RowIdx == 0) {
						if(AddItemToCurrentOrder(_data))
							do_notify_goods_list = true;
					}
					else if(CPM.CurrentOrder != null && CPM.CurrentOrder.TiList != null) {
						for(int i = 0; i < CPM.CurrentOrder.TiList.size(); i++) {
							Document.TransferItem ti = CPM.CurrentOrder.TiList.get(i);
							if(ti.RowIdx == _data.RowIdx) {
								if(_data.Set.Qtty > 0)
									CPM.CurrentOrder.TiList.get(i).Set.Qtty = _data.Set.Qtty;
								else
									CPM.CurrentOrder.TiList.remove(i);
								NotifyCurrentOrderChanged();
								do_notify_goods_list = true;
								break;
							}
						}
					}
					if(do_notify_goods_list) {
						CommonPrereqModule.TabEntry te = SearchTabEntry(CommonPrereqModule.Tab.tabGoods);
						if(te != null && te.TabView != null) {
							View v = te.TabView.getView();
							if(v != null && v instanceof ViewGroup) {
								View lv = ((ViewGroup)v).findViewById(R.id.orderPrereqGoodsListView);
								if(lv != null && lv instanceof RecyclerView) {
									RecyclerView.Adapter gva = ((RecyclerView)lv).getAdapter();
									if(gva != null)
										gva.notifyDataSetChanged(); // @todo Здесь надо обновлять только одну строку списка товаров
								}
							}
						}
					}
				}
				break;
			case SLib.EV_SVCQUERYRESULT:
				if(subj != null && subj instanceof StyloQApp.InterchangeResult) {
					StyloQApp.InterchangeResult ir = (StyloQApp.InterchangeResult)subj;
					if(ir.OriginalCmdItem != null && ir.OriginalCmdItem.Name.equalsIgnoreCase("PostDocument")) {
						CPM.Locker_CommitCurrentDocument = false;
						if(ir.ResultTag == StyloQApp.SvcQueryResult.SUCCESS) {
							CPM.MakeCurrentDocList((StyloQApp)getApplication());
							CPM.CurrentOrder = null;
							NotifyCurrentOrderChanged();
							GotoTab(CommonPrereqModule.Tab.tabOrders, R.id.orderPrereqOrderListView, -1, -1);
							CPM.SetTabVisibility(CommonPrereqModule.Tab.tabCurrentOrder, View.GONE);
						}
						else {

						}
					}
				}
				break;
		}
		return result;
	}
}