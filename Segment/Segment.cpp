#include "stdafx.h"
#include "Segment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CSegment, CObject, 0);
CSegment::CSegment()
{
	Sro_points.resize(11);
	Sri_points.resize(10);
	Slo_points.resize(11);
	Sli_points.resize(10);

	m_Selected = false;
	m_Id = 0;
	m_LwebThick = 0.5;
	m_RwebThick = 0.5;
	m_RplateThick = 0.39;
	m_LplateThick = 0.36;
	m_RlHeight = 3.54;
	m_LlHeight = 3.3;
	m_Length = 4.5;
	m_Volume = GetVolume();

	_root = new osg::Group;
	_callback = new CSegmentCallback(this);
}

CSegment::~CSegment()
{

}

double CSegment::GetVolume()
{
	double Vo,Vi,Slo,Sli,Sro,Sri;
	Slo = GetSlo();
	Sli = GetSli();
	Sro = GetSro();
	Sri = GetSri();
	Vo = (m_Length / 3) * (Slo + Sro + sqrt(Slo * Sro));
	Vi = (m_Length / 3) * (Sli + Sri + sqrt(Sli * Sri));
	return Vo - Vi;
}

double CSegment::GetSlo()
{
	CPoint2d opt[11];
	double Slo;
	 

	//左边外轮廓(已校对)
	opt[0].x = 0;
	opt[0].y = 0;

	opt[1].x = -(crossSection.m_TboxWidth / 2);
	opt[1].y = -(crossSection.m_TboxWidth / 2 * crossSection.m_SlpVal);

	opt[2].x = opt[1].x;
	opt[2].y = opt[1].y - crossSection.m_A;

	opt[3].x = opt[2].x + crossSection.m_A1;
	opt[3].y = opt[2].y - crossSection.m_B1;

	opt[4].x = opt[3].x + crossSection.m_A2;
	opt[4].y = opt[3].y - crossSection.m_B2;

	opt[5].x = opt[4].x;
	opt[5].y = -m_LlHeight - crossSection.m_DboxWidth * crossSection.m_SlpVal / 2;

	opt[6].x = opt[5].x + crossSection.m_DboxWidth;
	opt[6].y = opt[5].y;

	opt[10].x = crossSection.m_TboxWidth / 2;
	if (!crossSection.m_SlpType.Compare((LPCTSTR)_T("双向坡")))
		opt[10].y = -crossSection.m_TboxWidth / 2 * crossSection.m_SlpVal;
	else
		opt[10].y = crossSection.m_TboxWidth / 2 * crossSection.m_SlpVal;
	opt[9].x = opt[10].x;
	opt[9].y = opt[10].y - crossSection.m_A;
	opt[8].x = opt[9].x - crossSection.m_A1;
	opt[8].y = opt[9].y - crossSection.m_B8;
	opt[7].x = opt[8].x - crossSection.m_A2;
	opt[7].y = opt[8].y - crossSection.m_B7;
	for (int i = 0; i < 11; ++i)
		Slo_points.push_back(opt[i]);
	Slo = Yjmath.GetArea(Slo_points.size(), Slo_points);
	return Slo;
	//（校对完）
}

double CSegment::GetSli()
{
	CPoint2d ipt[10];
	double Sli;
	double avrgAPT = (m_LwebThick + m_RwebThick) / 2;

	//左边内轮廓(已校对)
	ipt[0].x = -crossSection.m_B / 2;
	if (!crossSection.m_SlpType.Compare((LPCTSTR)_T("双向坡")))
		ipt[0].y = -crossSection.m_H;
	else
		ipt[0].y = -crossSection.m_H - crossSection.m_B * crossSection.m_SlpVal / 2;
	ipt[1].x = ipt[0].x - crossSection.m_A4;
	ipt[1].y = ipt[0].y - crossSection.m_B4;

	ipt[2].x = -(crossSection.m_DboxWidth / 2 - avrgAPT);
	ipt[2].y = ipt[1].y - crossSection.m_B3 *(ipt[1].x - ipt[2].x) / crossSection.m_A3;

	ipt[3].x = ipt[2].x;
	ipt[3].y = -m_LlHeight - crossSection.m_DboxWidth * crossSection.m_SlpVal / 2 + m_LplateThick + crossSection.m_B9;

	ipt[4].x = ipt[3].x + crossSection.m_B9;
	ipt[4].y = ipt[3].y - crossSection.m_B9;

	ipt[9].x = crossSection.m_B / 2;
	if (!crossSection.m_SlpType.Compare((LPCTSTR)_T("双向坡")))
		ipt[9].y = -crossSection.m_H;
	else
		ipt[9].y = -crossSection.m_H + crossSection.m_B * crossSection.m_SlpVal / 2;

	ipt[8].x = ipt[9].x + crossSection.m_A4;
	ipt[8].y = ipt[9].y - crossSection.m_B5;

	ipt[7].x = crossSection.m_DboxWidth / 2 - avrgAPT;
	ipt[7].y = ipt[8].y + crossSection.m_B6 *(ipt[8].x - ipt[7].x) / crossSection.m_A3;

	ipt[6].x = ipt[7].x;
	ipt[6].y = ipt[3].y;

	ipt[5].x = ipt[6].x - crossSection.m_B9;
	ipt[5].y = ipt[4].y;

	for (int i = 0; i < 10; ++i)
		Sli_points.push_back(ipt[i]);

	Sli = Yjmath.GetArea(Sli_points.size(), Sli_points);
	return Sli;
	//（校对完）
}

double CSegment::GetSro()
{
	CPoint2d opt[11];
	double Sro;


	//右边外轮廓（已校对）
	opt[0].x = 0;
	opt[0].y = 0;

	opt[1].x = -crossSection.m_TboxWidth / 2;
	opt[1].y = -crossSection.m_TboxWidth / 2 * crossSection.m_SlpVal;

	opt[2].x = opt[1].x;
	opt[2].y = opt[1].y - crossSection.m_A;

	opt[3].x = opt[2].x + crossSection.m_A1;
	opt[3].y = opt[2].y - crossSection.m_B1;

	opt[4].x = opt[3].x + crossSection.m_A2;
	opt[4].y = opt[3].y - crossSection.m_B2;

	opt[5].x = opt[4].x;
	opt[5].y = -m_RlHeight - crossSection.m_DboxWidth * crossSection.m_SlpVal / 2;

	opt[6].x = opt[5].x + crossSection.m_DboxWidth;
	opt[6].y = opt[5].y;

	opt[10].x = crossSection.m_TboxWidth / 2;
	if (!crossSection.m_SlpType.Compare((LPCTSTR)_T("双向坡")))
		opt[10].y = -crossSection.m_TboxWidth / 2 * crossSection.m_SlpVal;
	else
		opt[10].y = crossSection.m_TboxWidth / 2 * crossSection.m_SlpVal;
	opt[9].x = opt[10].x;
	opt[9].y = opt[10].y - crossSection.m_A;

	opt[8].x = opt[9].x - crossSection.m_A1;
	opt[8].y = opt[9].y - crossSection.m_B8;

	opt[7].x = opt[8].x - crossSection.m_A2;
	opt[7].y = opt[8].y - crossSection.m_B7;

	for (int i = 0; i < 11; ++i)
		Sro_points.push_back(opt[i]);
	Sro = Yjmath.GetArea(Sro_points.size(), Sro_points);
	return Sro;
	//（校对完）

}

double CSegment::GetSri()
{
	CPoint2d ipt[10];
	double Sri;
	double avrgAPT = (m_LwebThick + m_RwebThick) / 2;

	//右边内轮廓
	ipt[0].x = -crossSection.m_B / 2;
	if (!crossSection.m_SlpType.Compare((LPCTSTR)_T("双向坡")))
		ipt[0].y = -crossSection.m_H;
	else
		ipt[0].y = -crossSection.m_H - crossSection.m_B * crossSection.m_SlpVal / 2;
	ipt[1].x = ipt[0].x - crossSection.m_A4;
	ipt[1].y = ipt[0].y - crossSection.m_B4;
	ipt[2].x = -(crossSection.m_DboxWidth / 2 - avrgAPT);
	ipt[2].y = ipt[1].y - crossSection.m_B3 *(ipt[1].x - ipt[2].x) / crossSection.m_A3;

	ipt[3].x = ipt[2].x;
	ipt[3].y = -m_RlHeight - crossSection.m_DboxWidth * crossSection.m_SlpVal / 2 + m_RplateThick + crossSection.m_B9;

	ipt[4].x = ipt[3].x + crossSection.m_B9;
	ipt[4].y = ipt[3].y - crossSection.m_B9;

	ipt[9].x = crossSection.m_B / 2;
	if (!crossSection.m_SlpType.Compare((LPCTSTR)_T("双向坡")))
		ipt[9].y = -crossSection.m_H;
	else
		ipt[9].y = -crossSection.m_H + crossSection.m_B * crossSection.m_SlpVal / 2;
	ipt[8].x = ipt[9].x + crossSection.m_A4;
	ipt[8].y = ipt[9].y - crossSection.m_B5;

	ipt[7].x = crossSection.m_DboxWidth / 2 - avrgAPT;
	ipt[7].y = ipt[8].y + crossSection.m_B6 *(ipt[8].x - ipt[7].x) / crossSection.m_A3;
	ipt[6].x = ipt[7].x;
	ipt[6].y = ipt[3].y;
	ipt[5].x = ipt[6].x - crossSection.m_B9;
	ipt[5].y = ipt[4].y;
	for (int i = 0; i < 10; ++i)
		Sri_points.push_back(ipt[i]);
	Sri = Yjmath.GetArea(Sri_points.size(), Sri_points);
	return Sri;
}

void CSegment::SetRoadData(CRoadData* road)
{
	m_Road = road;
}

void CSegment::SetSection(CStdSection* sect)
{
	m_Section = sect;
}

void CSegment::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar
			<< m_Id//节段编号
			<< m_Length//长度
			<< m_AddLength//累计长度
			<< m_LlHeight//左高度小
			<< m_LrHeight//左高度大
			<< m_RlHeight//右高度小
			<< m_RrHeight//右高度大
			<< m_LplateThick//左厚
			<< m_RplateThick//右厚
			<< m_LwebThick//左腹板厚
			<< m_RwebThick//右腹板厚
			<< m_LdesignElev//左设计高程
			<< m_RdesignElev//右设计高程
			<< m_LlwebElev//左上高程小
			<< m_LrwebElev//左上高程大
			<< m_RlwebElev//右上高程小
			<< m_RrwebElev//右上高程大
			<< m_LbottomElev1//左下高程1
			<< m_LbottomElev2//左下高程2
			<< m_RbottomElev1//右下高程1
			<< m_RbottomElev2//右下高程2
			<< m_Volume//体积
			<< anti_crackRebar;//防裂钢筋

		//存储外轮廓点集
		for (int i = 0; i < 11; ++i)
		{
			ar << Slo_points[i].x << Slo_points[i].y;//存储左外轮廓点集
			ar << Sro_points[i].x << Sro_points[i].y;//存储右外轮廓点集
		}
		//存储内轮廓点集
		for (int i = 0; i < 10; ++i)
		{
			ar << Sli_points[i].x << Sli_points[i].y;//存储左内轮廓点集
			ar << Sri_points[i].x << Sri_points[i].y;//存储右内轮廓点集
		}
	}
	else{
		ar
			>> m_Id//节段编号
			>> m_Length//长度
			>> m_AddLength//累计长度
			>> m_LlHeight//左高度小
			>> m_LrHeight//左高度大
			>> m_RlHeight//右高度小
			>> m_RrHeight//右高度大
			>> m_LplateThick//左厚
			>> m_RplateThick//右厚
			>> m_LwebThick//左腹板厚
			>> m_RwebThick//右腹板厚
			>> m_LdesignElev//左设计高程
			>> m_RdesignElev//右设计高程
			>> m_LlwebElev//左上高程小
			>> m_LrwebElev//左上高程大
			>> m_RlwebElev//右上高程小
			>> m_RrwebElev//右上高程大
			>> m_LbottomElev1//左下高程1
			>> m_LbottomElev2//左下高程2
			>> m_RbottomElev1//右下高程1
			>> m_RbottomElev2//右下高程2
			>> m_Volume//体积
			>> anti_crackRebar;//防裂钢筋

		//读取外轮廓点集
		for (int i = 0; i < 11; ++i)
		{
			ar >> Slo_points[i].x >> Slo_points[i].y;//读取左外轮廓点集
			ar >> Sro_points[i].x >> Sro_points[i].y;//读取右外轮廓点集
		}
		//读取内轮廓点集
		for (int i = 0; i < 10; ++i)
		{
			ar >> Sli_points[i].x >> Sli_points[i].y;//读取左内轮廓点集
			ar >> Sri_points[i].x >> Sri_points[i].y;//读取右内轮廓点集
		}
	}
}

void CSegment::GetModelAttrib()
{
	AfxMessageBox(_T("节段"));
}




void CSegmentCallback::DoUpdate()
{
	CClassGeode* geode = NULL;
	geode = UpdateScene(_segment->m_LeftStake, _segment->m_LdesignElev, min(_segment->m_LlHeight, _segment->m_LrHeight),
		_segment->m_RightStake, _segment->m_RdesignElev, min(_segment->m_RlHeight, _segment->m_RrHeight));
	_root->addChild(new PickModel(geode));
}

CClassGeode * CSegmentCallback::UpdateScene(double leftStake, double leftElev, double leftHeight, double rightStake, double rightElev, double rightHeight)
{
	CClassGeode* geode = new CClassGeode(_segment);

	osg::ref_ptr<osg::Vec3Array> leftOuter = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> leftInner = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> rightOuter = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> rightInner = new osg::Vec3Array();

	osg::ref_ptr<osg::Geometry> leftGeometry = _segment->m_Section->Draw(leftStake, 0.0, leftElev, leftHeight, leftOuter, leftInner);
	osg::ref_ptr<osg::Vec3Array> leftNormal = new osg::Vec3Array();
	leftNormal->push_back(osg::Vec3(-1.0, 0.0, 0.0));
	leftGeometry->setNormalArray(leftNormal, osg::Array::BIND_OVERALL);
	osg::ref_ptr<osg::Vec4Array> leftColor = new osg::Vec4Array();
	leftColor->push_back(osg::Vec4(0.5, 0.5, 0.5, 1.0f));
	leftGeometry->setColorArray(leftColor, osg::Array::BIND_OVERALL);
	geode->addDrawable(leftGeometry);

	osg::ref_ptr<osg::Geometry> rightGeometry = _segment->m_Section->Draw(rightStake, 0.0, rightElev, rightHeight, rightOuter, rightInner);
	osg::ref_ptr<osg::Vec3Array> rightNormal = new osg::Vec3Array();
	rightNormal->push_back(osg::Vec3(1.0, 0.0, 0.0));
	rightGeometry->setNormalArray(rightNormal, osg::Array::BIND_OVERALL);
	osg::ref_ptr<osg::Vec4Array> rightColor = new osg::Vec4Array();
	rightColor->push_back(osg::Vec4(0.5, 0.5, 0.5, 1.0f));
	rightGeometry->setColorArray(rightColor, osg::Array::BIND_OVERALL);
	geode->addDrawable(rightGeometry);

	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(*leftOuter);
	vertices->insert(vertices->end(), leftInner->begin(), leftInner->end());
	vertices->insert(vertices->end(), rightOuter->begin(), rightOuter->end());
	vertices->insert(vertices->end(), rightInner->begin(), rightInner->end());

	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.3, 0.5, 0.5, 1.0f));
	geometry->setVertexArray(vertices);
	geometry->setColorArray(colors, osg::Array::BIND_OVERALL);

	//修改顶点值绘制轮廓线（扩大1.2倍）
	osg::ref_ptr<osg::Vec3Array> outVertices = new osg::Vec3Array;
	for (unsigned int i = 0; i < vertices->size(); ++i)
	{
		double x = vertices->at(i).x()*1.0001f - 1.05f;
		double y = vertices->at(i).y()*1.06f;
		double z = vertices->at(i).z()*1.0015 - 1.143f;
		outVertices->push_back(osg::Vec3(x, y, z));
	}

	osg::ref_ptr<osg::Geometry> outgm = new osg::Geometry();
	osg::ref_ptr<osg::Vec4Array> outcl = new osg::Vec4Array();
	outcl->push_back(osg::Vec4(0.0f, 0.3f, 0.8f, 1.0f));
	outgm->setColorArray(outcl, osg::Array::BIND_OVERALL);
	outgm->setVertexArray(outVertices);
	osg::ref_ptr<osg::DrawElementsUInt> outline = nullptr;
	for (int i = 0; i < 10; i++)
	{
		outline = new osg::DrawElementsUInt(osg::PrimitiveSet::LINE_LOOP, 0);
		outline->push_back(i);
		outline->push_back(22 + i);
		outline->push_back(22 + i + 1);
		outline->push_back(i + 1);
		outgm->addPrimitiveSet(outline);
	}
	outline = new osg::DrawElementsUInt(osg::PrimitiveSet::LINE_LOOP, 0);
	outline->push_back(10);
	outline->push_back(32);
	outline->push_back(22);
	outline->push_back(0);
	outgm->addPrimitiveSet(outline);

	osg::ref_ptr<osg::DrawElementsUInt> out = NULL;
	for (int i = 0; i < 10; i++)
	{
		out = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
		out->push_back(i);
		out->push_back(22 + i);
		out->push_back(22 + i + 1);
		out->push_back(i + 1);
		geometry->addPrimitiveSet(out);
	}
	out = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	out->push_back(10);
	out->push_back(32);
	out->push_back(22);
	out->push_back(0);
	geometry->addPrimitiveSet(out);
	osg::ref_ptr<osg::DrawElementsUInt> in = NULL;
	for (int i = 11; i < 21; i++)
	{
		in = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
		in->push_back(i + 1);
		in->push_back(22 + i + 1);
		in->push_back(22 + i);
		in->push_back(i);
		geometry->addPrimitiveSet(in);
	}
	in = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	in->push_back(11);
	in->push_back(33);
	in->push_back(43);
	in->push_back(21);
	geometry->addPrimitiveSet(in);

	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	normal->push_back(osg::Vec3(0.0, 0.0, 1.0));
	normal->push_back(osg::Vec3(0.0, 1.0, 0.0));
	normal->push_back(osg::Vec3(0.0, 0.5, -0.866));
	normal->push_back(osg::Vec3(0.0, 0.866, -0.5));
	normal->push_back(osg::Vec3(0.0, 1.0, 0.0));
	normal->push_back(osg::Vec3(0.0, 0.0, -1.0));
	normal->push_back(osg::Vec3(0.0, -1.0, 0.0));
	normal->push_back(osg::Vec3(0.0, -0.866, -0.5));
	normal->push_back(osg::Vec3(0.0, -0.5, -0.866));
	normal->push_back(osg::Vec3(0.0, -1.0, 0.0));
	normal->push_back(osg::Vec3(0.0, 0.0, 1.0));

	normal->push_back(osg::Vec3(0.0, 0.0, -1.0));
	normal->push_back(osg::Vec3(0.0, -0.5, -0.866));
	normal->push_back(osg::Vec3(0.0, -0.866, -0.5));
	normal->push_back(osg::Vec3(0.0, -1.0, 0.0));
	normal->push_back(osg::Vec3(0.0, -0.707, 0.707));
	normal->push_back(osg::Vec3(0.0, 0.0, 1.0));
	normal->push_back(osg::Vec3(0.0, 0.707, 0.707));
	normal->push_back(osg::Vec3(0.0, 1.0, 0.0));
	normal->push_back(osg::Vec3(0.0, 0.866, -0.5));
	normal->push_back(osg::Vec3(0.0, 0.5, -0.866));
	normal->push_back(osg::Vec3(0.0, 0.0, -1.0));
	geometry->setNormalArray(normal, osg::Array::BIND_PER_PRIMITIVE_SET);

	geode->addDrawable(geometry);
	osg::ref_ptr<osgFX::Scribe> scribe = new osgFX::Scribe;
	scribe->setWireframeColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	scribe->setWireframeLineWidth(0.8f);
	scribe->addChild(geode);
	//geode->addDrawable(outgm);
	return (CClassGeode*)scribe.release();
}
