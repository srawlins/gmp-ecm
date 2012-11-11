require File.join(File.dirname(__FILE__), 'spec_helper')

describe GMP::Z, "#ecm_factor" do
  it "should find one of the expected factors of 2^71 - 1" do
    @a = GMP::Z(2) ** 71 - 1
    @factors = [GMP::Z(228479), GMP::Z(48544121), GMP::Z(212885833),
                GMP::Z(11091312221959), GMP::Z("48639942238007"),
                GMP::Z("2361183241434822606847")]
    8.times do
      found, factor = @a.ecm_factor(1e6)
      found.should == 1
      @factors.should include(factor)
    end
  end
end
